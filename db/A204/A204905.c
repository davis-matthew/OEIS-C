/*
 * A204905  Least k such that n divides k^2-j^2 for some j satisfying 1<=j<k.
 * https://oeis.org/A204905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A204905: minimal k such that
   n divides k² – j² for some integer j with 1 ≤ j < k. */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* unused for this sequence */

    seq_t N = (seq_t)n;
    bool found = false;
    seq_t best = 0;

    /* iterate over possible values of a = k – j (1 ≤ a < k) */
    for (idx_t aval = 1; aval <= n; ++aval) {
        seq_t a_seq = (seq_t)aval;

        /* d = n / gcd(a,n).  Then b must be a positive multiple of d. */
        seq_t g = gcd(a_seq, N);
        seq_t d = N / g;

        /* smallest b > a that is a multiple of d */
        seq_t t = (a_seq + d) / d;          /* ceil((a+1)/d)          */
        seq_t b = d * t;

        /* ensure a and b have the same parity (j integer) */
        if ((a_seq & 1) != (b & 1)) {
            if (d & 1) {                    /* d odd → parity can be flipped */
                b += d;
            } else {                        /* d even → impossible for this a */
                continue;
            }
        }

        /* compute k = (a + b)/2 */
        seq_t k = (a_seq + b) / 2;

        if (!found || k < best) {
            best = k;
            found = true;
        }
    }

    return best;
}
