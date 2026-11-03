/*
 * A186155  Rank of n^2 when {(1/8)i^3: i>=1} and {j^2>: j>=1} are jointly
 *          ranked with (1/8)i^3 after j^2 when (1/8)i^3=j^2. Complement of
 *          A186154.
 * https://oeis.org/A186155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((8*n^2 - 1)^(1/3))
 * Range:     u8 n <= 5, u16 n <= 75, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the rank of n² in the interleaving of (i³)/8 and j²,
   with j² coming before (i³)/8 when equal. */

#include <stdint.h>
#include <stdbool.h>

/* Integer cube‑root: largest integer r with r³ ≤ x. */
static idx_t icbrt_seq_t(seq_t x)
{
    idx_t low = 0, high = 1;
    /* Grow high until high³ > x */
    while ((seq_t)high * high * high <= x)
        high <<= 1;          /* high stays well below the true root for ^64‑bit x */

    /* Binary search [low, high) */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        seq_t mc = (seq_t)mid * mid * mid;
        if (mc <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1, but return 0 safely */
        return 0;

    seq_t nn = (seq_t)n * (seq_t)n;    /* n² */
    seq_t v  = 8 * nn - 1;             /* 8·n² – 1 */
    idx_t c  = icbrt_seq_t(v);         /* floor((8·n² – 1)^(1/3)) */

    return (seq_t)n + (seq_t)c;        /* rank of n² */
}
