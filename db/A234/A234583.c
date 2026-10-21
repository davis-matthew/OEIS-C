/*
 * A234583  Let S_n = 0 followed by base-8 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   mult_overflow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by iterating over blocks defined by base‑8 block lengths. */
#include <stdint.h>
#include <limits.h>
#include "../utils/mult_overflow.h"

seq_t a(idx_t n) {
    unsigned long long idx = (unsigned long long)n;   /* digit index */
    unsigned long long d = 1;                         /* current block length (base‑8 digits) */
    unsigned long long pow8 = 1;                      /* 8^(d‑1) */

    while (1) {
        /* Count of numbers in this block */
        unsigned long long count_d;
        if (d == 1)
            count_d = 8ULL;            /* numbers 0 … 7 */
        else
            count_d = 7ULL * pow8;    /* 7*8^(d‑1) */

        unsigned long long len_d = d + 1ULL;          /* length of each S_n */

        unsigned long long prod;
        if (!mult_overflow(count_d, len_d, &prod) && idx >= prod) {
            /* Entire block lies before the target digit */
            idx -= prod;
            d++;
            pow8 *= 8ULL;               /* move to next power of 8 */
            continue;
        }

        /* The target digit is within this block.  Compute it. */
        unsigned long long start = (d == 1) ? 0ULL : pow8;
        unsigned long long lenS = len_d;
        unsigned long long k_index = idx / lenS;        /* index of n inside the block */
        unsigned long long digit_within = idx % lenS;   /* position inside S_n (0‑based) */
        unsigned long long n_k = start + k_index;

        if (digit_within == 0ULL) {
            /* First digit of S_n is always 0 */
            return (seq_t)0;
        } else {
            unsigned long long pos = digit_within - 1ULL;   /* 0‑based position in reversed digits */
            unsigned long long pow_pos = 1ULL;
            for (unsigned long long i = 0; i < pos; ++i)
                pow_pos *= 8ULL;                       /* 8^pos */

            unsigned long long digit = (n_k / pow_pos) % 8ULL;
            return (seq_t)digit;
        }
    }
}
