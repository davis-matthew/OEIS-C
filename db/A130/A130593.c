/*
 * A130593  Evil semiprimes.
 * https://oeis.org/A130593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Evil semiprimes (A130593) – products of two primes,
   each product has an even number of 1's in its binary expansion. */

#include <stdint.h>
#include <stdlib.h>

/* Helper to compute even popcount for uint64_t values */
static int popcount_even(uint64_t v)
{
    int c = 0;
    while (v) {
        c += v & 1;
        v >>= 1;
    }
    return (c & 1) == 0;
}

/* Build primes and smallest prime factor table up to LIMIT,
   then collect all evil semiprimes into a dynamic array. */
static void build_evil_semiprimes(seq_t **evils, size_t *cnt, size_t *cap,
                                  uint32_t limit)
{
    /* Smallest prime factor array */
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) return; /* allocation failure – not expected in tests */

    /* Sieve for spf */
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    /* Allocate array for evil semiprimes */
    size_t initial_cap = 131072;
    *evils = (seq_t *)malloc(initial_cap * sizeof(seq_t));
    *cap   = initial_cap;
    *cnt   = 0;

    /* Scan numbers, collect evil semiprimes */
    for (uint32_t n = 4; n <= limit; ++n) {
        uint32_t p = spf[n];
        uint32_t q = n / p;
        /* n is semiprime if q is prime (spf[q]==q) */
        if (q >= 2 && spf[q] == q) {
            if (popcount_even(n)) {
                if (*cnt == *cap) {
                    *cap *= 2;
                    *evils = (seq_t *)realloc(*evils, *cap * sizeof(seq_t));
                }
                (*evils)[*cnt] = (seq_t)n;
                ++(*cnt);
            }
        }
    }

    /* Free spf table */
    free(spf);
}

/* Static state for one-time initialization */
static seq_t *evil_semi = NULL;
static size_t   evil_cnt = 0;
static size_t   evil_cap = 0;
static const uint32_t GLOBAL_LIMIT = 2000000; /* sufficient for n up to 10000 */

seq_t a(idx_t n)
{
    if (!evil_semi) {
        build_evil_semiprimes(&evil_semi, &evil_cnt, &evil_cap, GLOBAL_LIMIT);
    }

    /* n is 1‑based, array 0‑based */
    if (n == 0 || (size_t)n > evil_cnt) {
        /* Out of bounds – return 0 (should not happen for valid tests) */
        return (seq_t)0;
    }
    return evil_semi[n - 1];
}
