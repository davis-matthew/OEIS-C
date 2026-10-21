/*
 * A231636  Number of n X 2 0..2 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to itself plus one mod 3, with upper left element
 *          zero (rock paper and scissors drawn positions).
 * https://oeis.org/A231636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + 4*a(n-3) - 14*a(n-4) * + 48*a(n-5)
 *            - 48*a(n-6) + 16*a(n-7); * Initial values: * a(1)=1, a(2)=5,
 *            a(3)=25, a(4)=124, a(5)=599, a(6)=2907, a(7)=14098
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Compute the n-th term of OEIS sequence A231636. */
seq_t a(idx_t n)
{
    /* The first seven terms are known explicitly. */
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)5,
        (seq_t)25,
        (seq_t)124,
        (seq_t)599,
        (seq_t)2907,
        (seq_t)14098
    };

    /* For n <= 7 return the pre‑computed value. */
    if (n <= 7)
        return init[n - 1];

    /* Keep the last seven values: a(n-7) … a(n-1) in this order. */
    seq_t prev[7];
    for (idx_t i = 0; i < 7; ++i)
        prev[i] = init[i];

    seq_t cur = 0;

    /* Iteratively apply the recurrence for k = 8 … n. */
    for (idx_t k = 8; k <= n; ++k) {
        /* a(k) = 7*a(k-1) - 11*a(k-2) + 4*a(k-3) - 14*a(k-4)
         *        + 48*a(k-5) - 48*a(k-6) + 16*a(k-7)
         */
        cur = 7 * prev[6]
            - 11 * prev[5]
            + 4 * prev[4]
            - 14 * prev[3]
            + 48 * prev[2]
            - 48 * prev[1]
            + 16 * prev[0];

        /* Shift the window: drop a(k-7), advance indices. */
        for (idx_t i = 0; i < 6; ++i)
            prev[i] = prev[i + 1];
        prev[6] = cur;
    }

    return cur;
}
