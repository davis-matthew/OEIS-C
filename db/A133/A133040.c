/*
 * A133040  Divisors of 1900.
 * https://oeis.org/A133040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all divisors of 1900, sort them, and return the n‑th one. */
#include <stdlib.h>

/* The key value to compute divisors for: 1900 = 2^2 * 5^2 * 19^1. */
static const int EXP_2 = 2;
static const int EXP_5 = 2;
static const int EXP_19 = 1;
static const seq_t PRIMES[3] = {2, 5, 19};

/* Simple comparison function for seq_t values. */
static int compare_seq(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Returns the n‑th divisor of 1900 in ascending order. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* n is 1‑based in OEIS. */
    static seq_t divisors[18];
    static int initialized = 0;

    if (!initialized) {
        size_t idx = 0;
        /* Enumerate all combinations of exponents. */
        for (int a = 0; a <= EXP_2; ++a) {
            for (int b = 0; b <= EXP_5; ++b) {
                for (int c = 0; c <= EXP_19; ++c) {
                    seq_t val = (seq_t)1;
                    for (int i = 0; i < a; ++i) val *= 2;
                    for (int i = 0; i < b; ++i) val *= 5;
                    if (c) val *= 19;
                    divisors[idx++] = val;
                }
            }
        }
        /* idx should now be 18.  Sort the array. */
        qsort(divisors, 18, sizeof(seq_t), compare_seq);
        initialized = 1;
    }

    if (n <= 18) return divisors[n - 1];
    return 0; /* n exceeds the number of divisors – return 0. */
}
