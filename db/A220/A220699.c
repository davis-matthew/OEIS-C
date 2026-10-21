/*
 * A220699  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n-4)*a(n-2) + 1.
 * https://oeis.org/A220699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 34, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A220699 (recurrence):
 *   a(0) = 0, a(1) = 0
 *   a(n) = a(n‑1) + (n-4)*a(n‑2) + 1   for n >= 2
 *   (equivalently a(n) = 2*a(n-1) + (n-5)*a(n-2) - (n-5)*a(n-3))
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1) {
        return (seq_t)0;
    }

    /* iterative computation; only the two previous values are needed */
    seq_t a_prev2 = (seq_t)0;      /* a(n-2) */
    seq_t a_prev1 = (seq_t)0;      /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient (i-4) may be negative for i < 4; cast to signed first */
        seq_t coeff = (seq_t)((int64_t)i - 4);
        a_n = a_prev1 + coeff * a_prev2 + (seq_t)1;
        a_prev2 = a_prev1;
        a_prev1 = a_n;
    }

    return a_n;
}
