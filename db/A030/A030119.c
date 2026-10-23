/*
 * A030119  a(n) = a(n-1) + a(n-2) + n, a(0) = a(1) = 1.
 * https://oeis.org/A030119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the A030119 sequence value for a given non‑negative index. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;        /* a(0) */
    seq_t a1 = (seq_t)1;        /* a(1) */
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        /* cast i to seq_t to keep operations in seq_t space */
        seq_t ni = (seq_t)i;
        ai = a1 + a0 + ni;     /* a(i) = a(i-1) + a(i-2) + i */
        a0 = a1;               /* shift window */
        a1 = ai;
    }
    return a1;                  /* a(n) */
}
