/*
 * A098770  Abundant numbers whose reversal is a deficient number.
 * https://oeis.org/A098770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implements sequence A098770:
 * a(n) = the nth integer x ≥ 12 such that x is an abundant number
 *         (sum of proper divisors > x) and reverse(x) is a deficient
 *         number (sum of proper divisors < reverse(x)).
 *
 * The function computes terms on demand by a simple linear search
 * powered by a pre‑computed sieve of primes up to 1 000 000.
 * Results are cached so that subsequent calls to any n are answered
 * in constant time.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to 1 000 000) ---------- */
#define PRIME_LIMIT 1000000
static uint32_t primes[80000];   /* there's 78 498 primes ≤ 1 000 000 */
static int primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;
    static bool composite[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT)
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = true;
        }
    }
}

/* ---------- Decimal reversal ---------- */
static uint64_t reverse_uint64(uint64_t v)
{
    uint64_t rev = 0;
    while (v) {
        rev = rev * 10 + v % 10;
        v /= 10;
    }
    return rev;
}

/* ---------- Divisor-sum helpers (abundant / deficient) ---------- */
static bool is_abundant(uint64_t n)
{
    unsigned __int128 threshold = (unsigned __int128)2 * n;
    unsigned __int128 sigma = 1;
    uint64_t temp = n;

    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            unsigned __int128 term = 1, pow = 1;
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
                term += pow;
            }
            sigma *= term;
            if (sigma > threshold) return true;   /* early exit */
        }
    }
    if (temp > 1) {
        unsigned __int128 term = 1 + (unsigned __int128)temp;
        sigma *= term;
        if (sigma > threshold) return true;
    }
    return sigma > threshold;
}

static bool is_deficient(uint64_t n)
{
    unsigned __int128 threshold = (unsigned __int128)2 * n - 1;   /* sigma <= 2*n-1 */
    unsigned __int128 sigma = 1;
    uint64_t temp = n;

    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            unsigned __int128 term = 1, pow = 1;
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
                term += pow;
            }
            sigma *= term;
            if (sigma > threshold) return false;    /* not deficient */
        }
    }
    if (temp > 1) {
        unsigned __int128 term = 1 + (unsigned __int128)temp;
        sigma *= term;
        if (sigma > threshold) return false;
    }
    return true;         /* sigma ≤ 2*n-1  ⇒ deficient */
}

/* ---------- Cache for sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 12;

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    init_primes();     /* ensure primes are ready once */

    /* generate terms until we reach the requested index */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (is_abundant(cand) && is_deficient(reverse_uint64(cand))) {
            if (cache_len + 1 > cache_cap) {             /* grow cache */
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);            /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;             /* store the term */
        }
    }
    return cache[(size_t)n - 1];   /* 1‑based index */
}
