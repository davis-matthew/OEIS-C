/*
 * A138799  Values of T(j) corresponding to least possible T(k) with
 *          T(k)-T(j)=n, where T(i)>0 are the triangular numbers A000217.
 * https://oeis.org/A138799
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the minimal triangular number T(j) such that
   there exists a k > j with T(k) - T(j) = n.
   Implements the formula derived from
   2n = (k-j)(k+j+1).  */
/* The function works for any integer type seq_t supplied by the build
   (unsigned 8‑, 16‑, 32‑, 64‑ or 128‑bit, or arbitrary precision). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values for n < 2 are irrelevant. */
    if (n < 2) return 0;

    /* 2n as a seq_t value. */
    seq_t D = (seq_t)n * 2;

    /* minimal j found so far (seq_t max as sentinel). */
    seq_t min_j = (seq_t)-1;

    /* Enumerate all divisor pairs (d, e) of D with d ≤ e. */
    for (idx_t d = 1; (seq_t)d * (seq_t)d <= D; ++d)
    {
        if (D % d != 0) continue;

        seq_t e = D / (seq_t)d;

        /* temp = e - d - 1 must be > 0 and even. */
        if (e > d + 1)
        {
            seq_t temp = e - (seq_t)d - 1;
            if ((temp & 1u) == 0)
            {
                seq_t j = temp / 2;
                if (j >= 1 && (min_j == (seq_t)-1 || j < min_j))
                {
                    min_j = j;
                    /* The smallest possible j is 1; stop if found. */
                    if (j == 1) break;
                }
            }
        }
    }

    if (min_j == (seq_t)-1) return 0;  /* No representation – should not happen. */

    /* Return the triangular number T(min_j). */
    return min_j * (min_j + 1) / 2;
}
