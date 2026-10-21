/*
 * A063452  Numbers k such that k - mu(k) is prime.
 * https://oeis.org/A063452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that k - μ(k) is prime. μ(k) is the Möbius
 *            function. The sequence contains the integers k ≥ 2 with this
 *            property
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* Standard headers ------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* 64‑bit deterministic Miller–Rabin primality test ----------------*/
static int isPrime64(uint64_t n) {
    if (n < 2) return 0;
    /* small primes */
    static const uint32_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return 1;
        if (n % small[i] == 0) return 0;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases sufficient for deterministic test on 64‑bit */
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,
                                     28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1) x = x * p % n;
            p = (__uint128_t)p * p % n;
            e >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        int comp = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = x * x % n;
            if (x == n-1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------ */
/* Prime sieve precomputation -----------------------------------*/
#define PRIME_LIMIT 2000000      /* sieve up to 2 000 000 (more than enough) */

static uint8_t *prime_flag = NULL;   /* 1 if prime, 0 otherwise */
static uint32_t *prime_list = NULL;  /* list of primes up to PRIME_LIMIT */
static size_t prime_cnt   = 0;
static bool initialized = false;

static void init_tables(void) {
    if (initialized) return;
    /* allocate flag array */
    prime_flag = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!prime_flag) exit(1);
    memset(prime_flag, 1, PRIME_LIMIT + 1);
    prime_flag[0] = prime_flag[1] = 0;

    /* simple Eratosthenes sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) prime_flag[j] = 0;
        }
    }
    /* allocate (and fill) prime list */
    prime_list = (uint32_t *)malloc(160000 * sizeof(uint32_t));
    if (!prime_list) exit(1);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_flag[i]) prime_list[prime_cnt++] = i;

    initialized = true;
}

/* ------------------------------------------------------------ */
/* Store the sequence terms (cached) ---------------------------*/
static seq_t *cache      = NULL;   /* cached values   */
static size_t  cache_cap = 0;
static size_t  cache_len = 0;      /* number of terms stored */

static void cache_ensure(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);
    cache = new_buf;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Helper: squarefree test and parity of ω(k) -----------------*/
static bool squarefree_and_parity(uint64_t n, int *parity_out) {
    uint64_t x = n;
    int parity = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            parity ^= 1;            /* one distinct prime factor found */
            x /= p;
            if (x % p == 0)         /* squared prime factor → not squarefree */
                return false;
        }
    }
    if (x > 1) parity ^= 1;      /* remaining prime factor */
    *parity_out = parity;
    return true;
}

/* ------------------------------------------------------------ */
/* Prime test using precomputed sieve or Miller–Rabin ---------*/
static int isPrimeLimited(uint64_t n) {
    if (n <= PRIME_LIMIT) return prime_flag[n];
    return isPrime64(n);
}

/* ------------------------------------------------------------ */
/* State keeping the next candidate integer -------------------*/
static uint64_t next_candidate = 2;   /* first k to test */

/* ------------------------------------------------------------ */
/* The sequence function --------------------------------------*/
seq_t a(idx_t n) {
    init_tables();

    while (cache_len < n) {
        /* test numbers until a new term is found */
        while (true) {
            /* Ensure current candidate is at least 2 */
            if (next_candidate < 2) next_candidate = 2;

            int parity;
            if (!squarefree_and_parity(next_candidate, &parity)) {
                ++next_candidate;
                continue;
            }

            /* parity == 0 -> μ = +1 : need k-1 prime
               parity == 1 -> μ = -1 : need k+1 prime */
            if (parity == 0) {
                if (isPrimeLimited(next_candidate - 1)) {
                    cache_ensure(cache_len + 1);
                    cache[cache_len++] = (seq_t)next_candidate;
                    ++next_candidate;
                    break;
                }
            } else {
                if (isPrimeLimited(next_candidate + 1)) {
                    cache_ensure(cache_len + 1);
                    cache[cache_len++] = (seq_t)next_candidate;
                    ++next_candidate;
                    break;
                }
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
