/*
 * A202332  Number of (n+1) X 6 binary arrays with consecutive windows of two
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 102, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7)   for n > 7
   Initial values:
   a(1)=64,  a(2)=191,  a(3)=478,  a(4)=1052,  a(5)=2102,  a(6)=3896,  a(7)=6800
*/

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first seven terms directly */
    static const seq_t base[7] = {64, 191, 478, 1052, 2102, 3896, 6800};

    if (n <= 7) {
        /* idx_t is 1-based; array indices are 0-based */
        return base[n - 1];
    }

    /* a[0] .. a[6] hold the last seven computed values */
    seq_t a_vals[7] = {base[0], base[1], base[2], base[3], base[4], base[5], base[6]};

    seq_t next;
    for (idx_t k = 8; k <= n; ++k) {
        /* Compute next value using only unsigned arithmetic.
           The recurrence can be rearranged as:
           next = (a[n-7] + 7*a[n-1] + 35*a[n-3] + 21*a[n-5])
                  - (21*a[n-2] + 35*a[n-4] + 7*a[n-6]) */
        seq_t pos = a_vals[0];                /* a[n-7] */
        pos += a_vals[6] * (seq_t)7;           /* 7*a[n-1] */
        pos += a_vals[4] * (seq_t)35;          /* 35*a[n-3] */
        pos += a_vals[2] * (seq_t)21;          /* 21*a[n-5] */

        seq_t neg = a_vals[5] * (seq_t)21;     /* 21*a[n-2] */
        neg += a_vals[3] * (seq_t)35;          /* 35*a[n-4] */
        neg += a_vals[1] * (seq_t)7;           /* 7*a[n-6] */

        /* Since for n > 7 the positive sum is always ≥ negative sum,
           subtraction is safe for unsigned types. */
        next = pos - neg;

        /* Shift the last seven values left to make room for the new one */
        for (int i = 0; i < 6; ++i) {
            a_vals[i] = a_vals[i + 1];
        }
        a_vals[6] = next;
    }

    /* The last element a_vals[6] now holds a(n) */
    return a_vals[6];
}
