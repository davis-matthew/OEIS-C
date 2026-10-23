/*
 * A191497  a(n+1) = 2*a(n) + A014017(n+5), a(0) = 0.
 * https://oeis.org/A191497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=0. Recurrence: a(n+4) = 2^n - a(n) (for n ≥
 *            0)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A191497. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 4, all terms are zero. */
    if (n < 4) return 0;

    /* Ring buffer holding a[i] for the last four indices. */
    seq_t buf[4] = {0, 0, 0, 0};

    /* Compute a[i] for i = 4 .. n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        buf[i & 3] = pow2(i - 4) - buf[(i - 4) & 3];
    }

    /* Return the desired term. */
    return buf[n & 3];
}
