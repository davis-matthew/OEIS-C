/*
 * A037283  Replace n with concatenation of its odd divisors.
 * https://oeis.org/A037283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 104, u64 n <= 314,
 *            u128 n <= 2834, big any n
 * Helpers:   cmp_seq_t, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037283:
 * a(n) = concatenation of the odd divisors of n in ascending order.
 * Example: n = 15 → odd divisors = 1, 3, 5, 15 → a(15) = 13515. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility functions provided by the build system. */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"     /* int digit_len(seq_t x); */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const void *a, const void *b); */

seq_t a(idx_t n)
{
    /* Obtain the odd part of n (remove all factors of 2). */
    seq_t odd = (seq_t)n;
    while ((odd % 2) == 0)
        odd /= 2;

    /* Enumerate all odd divisors of the odd part. */
    /* The number of divisors of an integer ≤ 10^4 is < 128; use a small array. */
    const size_t MAX_DIVS = 256;
    seq_t divs[MAX_DIVS];
    size_t divcnt = 0;

    for (idx_t i = 1; (seq_t)i * (seq_t)i <= odd; ++i) {
        if ((odd % i) == 0) {
            divs[divcnt++] = (seq_t)i;
            seq_t d2 = odd / (seq_t)i;
            if (d2 != (seq_t)i) {
                divs[divcnt++] = d2;
            }
        }
    }

    /* Sort the divisors in ascending order. */
    if (divcnt > 1)
        qsort(divs, divcnt, sizeof(seq_t), cmp_seq_t);

    /* Concatenate the sorted divisors to form the result. */
    seq_t result = 0;
    for (size_t k = 0; k < divcnt; ++k) {
        seq_t d = divs[k];
        seq_t mult = pow10((idx_t)digit_len(d));
        result = result * mult + d;
    }

    return result;
}
