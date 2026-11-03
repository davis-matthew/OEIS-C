/*
 * A133849  Least odd primitive abundant numbers with no factor 3 and with 5^n
 *          but not 5^(n+1) as a factor.
 * https://oeis.org/A133849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n * 7 * 11 * 13 * 17 * 19 * 23 * 29 for n ≥ 2. a(0) =
 *            20169691981106018776756331 (product of
 *            7,7,11,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67). a(1) =
 *            33426748355 (product of 5,7,11,13,17,19,23,29,31)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 15, big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utility functions for arithmetic on seq_t. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle special cases n = 0 and n = 1.  Each value is computed once. */
    if (n == 0) {
        /* static local to cache the computed value after first call */
        static seq_t val = 0;
        if (!val) {
            /* List of primes (with multiplicities) for a(0). */
            const uint32_t primes0[] = {
                7, 7, 11, 11, 13, 17, 19, 23, 29,
                31, 37, 41, 43, 47, 53, 59, 61, 67
            };
            size_t len = sizeof(primes0) / sizeof(primes0[0]);
            seq_t p = 1;
            for (size_t i = 0; i < len; ++i) {
                p = seq_mul(p, (seq_t)primes0[i]);
            }
            val = p;
        }
        return val;
    } else if (n == 1) {
        static seq_t val = 0;
        if (!val) {
            const uint32_t primes1[] = {5, 7, 11, 13, 17, 19, 23, 29, 31};
            size_t len = sizeof(primes1) / sizeof(primes1[0]);
            seq_t p = 1;
            for (size_t i = 0; i < len; ++i) {
                p = seq_mul(p, (seq_t)primes1[i]);
            }
            val = p;
        }
        return val;
    } else {
        /* For n ≥ 2 use the closed form: 5ⁿ * 215656441.  */
        seq_t pow5 = ipow_idx((seq_t)5, (idx_t)n);
        /* 215656441 = 7 * 11 * 13 * 17 * 19 * 23 * 29 */
        static const seq_t product = (seq_t)215656441;
        return seq_mul(pow5, product);
    }
}
