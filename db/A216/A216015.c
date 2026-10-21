/*
 * A216015  Primes that are the sum of 50001 consecutive primes.
 * https://oeis.org/A216015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that are the sum of 50001 consecutive primes (computed by sliding window over primes) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* utilities that may be used by the build system; we include the primality test we implement */
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------------ */
/*  Helpers for 64‑bit deterministic Miller–Rabin primality test       */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes filter */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n–1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64-bit integers */
    static const uint64_t bases[] =
        { 2ULL, 325ULL, 9375ULL, 28178ULL,
          450775ULL, 9780504ULL, 1795265022ULL };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1;
        __uint128_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1ULL;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* combine the fast 64‑bit test with the generic big‑int test */
static bool is_prime_seq(seq_t n)
{
    /* UINT64_MAX promotion works for all builtin and big‑int seq_t types */
    seq_t max64 = (seq_t)UINT64_MAX;
    if (n <= max64) {
        return is_prime_uint64((uint64_t)n);
    } else {
        return is_prime_big(n);
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence routine                                           */
/* ------------------------------------------------------------------ */
static const size_t WINDOW = 50001;          /* consecutive primes to sum   */
static const size_t PRIME_LIMIT = 20000000; /* sieve limit (≈2×10⁷)         */

static seq_t *terms = NULL;          /* cache of found sequence terms */
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* prime list */
static uint32_t *prime_list = NULL;
static size_t prime_len = 0;
static size_t prime_cap = 0;

static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* -------------------------------------------------------------
     * 1. Eratosthenes sieve up to PRIME_LIMIT
     * ------------------------------------------------------------- */
    uint8_t *is_comp = (uint8_t*)malloc(PRIME_LIMIT + 1);
    if (!is_comp) return;  /* low‑memory guard */

    is_comp[0] = is_comp[1] = 1;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* estimate number of primes (~N / ln N) and allocate array */
    prime_cap = PRIME_LIMIT / 10 + 10; /* ~1.2 M for 20 M */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { free(is_comp); return; }

    prime_len = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_len == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list,
                                                  prime_cap * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(prime_list); return; }
                prime_list = tmp;
            }
            prime_list[prime_len++] = (uint32_t)i;
        }
    }
    free(is_comp);

    if (prime_len < WINDOW) return; /* unlikely but safe */

    /* -------------------------------------------------------------
     * 2. Sliding window sums, collect those that are prime
     * ------------------------------------------------------------- */
    terms_cap = 8192;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) return;

    terms_len = 0;
    seq_t sum = 0;
    for (size_t i = 0; i < WINDOW; ++i)
        sum += (seq_t)prime_list[i];

    for (size_t start = 0;
         (start + WINDOW) <= prime_len;
         ++start) {
        if (is_prime_seq(sum)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(terms,
                                            terms_cap * sizeof(seq_t));
                if (!tmp) return;
                terms = tmp;
            }
            terms[terms_len++] = sum;
        }
        if (start + WINDOW < prime_len) {
            sum -= (seq_t)prime_list[start];
            sum += (seq_t)prime_list[start + WINDOW];
        }
    }
}

/* -------------------------------------------------------------- */
/*  Public entry point                                           */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;               /* offset is 1, unused case */

    idx_t idx = n - 1;                   /* 0‑based index into cache */
    if (idx < terms_len) return terms[idx];

    /* index beyond pre‑computed range – return 0 (undefined) */
    return 0;
}
