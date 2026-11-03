/*
 * A037283  Replace n with concatenation of its odd divisors.
 * https://oeis.org/A037283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 104, u64 n <= 314,
 *            u128 n <= 2834, big any n
 * Validated: standard, pass, 67 terms through n = 9042 (2026-11-03)
 * Helpers:   mem, num_digits
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
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

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
        mem_qsort(divs, divcnt, sizeof(seq_t), cmp_seq_t);

    /* Concatenate the sorted divisors to form the result. */
    seq_t result = 0;
    for (size_t k = 0; k < divcnt; ++k) {
        seq_t d = divs[k];
        seq_t mult = pow10((idx_t)num_digits(d));
        result = result * mult + d;
    }

    return result;
}
