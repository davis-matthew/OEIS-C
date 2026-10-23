/*
 * A046203  Even numbers in the triangle of denominators in Leibniz's Harmonic
 *          Triangle.
 * https://oeis.org/A046203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th even denominator in Leibniz's Harmonic Triangle.
   Denominators of row r are r * C(r-1, k) for 0 ≤ k < r.
   The function scans rows counting the even denominators until it
   reaches the desired position, then computes the appropriate
   binomial coefficient and returns the product. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cumulative number of even denominators seen so far */
    idx_t cum = 0;

    for (idx_t r = 1; ; ++r) {
        /* number of even entries in this row */
        idx_t evens;
        if ((r & 1) == 0) {           /* r even → all entries even */
            evens = r;
        } else {                     /* r odd → subtract odd binomial coeffs */
            /* number of odd binomial coefficients in row r-1 is
               2^popcount(r-1) (Lucas' theorem) */
            unsigned bits = __builtin_popcountll(r - 1);
            idx_t odd = (idx_t)1 << bits;   /* bits is small (≤ 64) for √n range */
            evens = r - odd;
        }

        /* check if the target resides in this row */
        if (cum + evens >= n) {
            /* offset (0‑based) of the required even entry within this row */
            idx_t offset = n - cum - 1;
            idx_t k = 0;
            seq_t bin = 1;             /* C(r-1, 0) */

            if ((r & 1) == 0) {
                /* all entries are even; directly compute the k‑th binomial */
                k = offset;
                for (idx_t i = 0; i < k; ++i) {
                    bin = bin * (seq_t)(r - 1 - i) / (seq_t)(i + 1);
                }
                return (seq_t)r * bin;
            } else {
                /* r odd: need to skip odd binomial coefficients */
                idx_t cnt_even = 0;
                for (k = 0; k < r; ++k) {
                    if ((((seq_t)bin) & 1ULL) == 0) {
                        if (cnt_even == offset) {
                            return (seq_t)r * bin;
                        }
                        ++cnt_even;
                    }
                    /* prepare next binomial coefficient, if needed */
                    if (k + 1 < r) {
                        bin = bin * (seq_t)(r - 1 - k) / (seq_t)(k + 1);
                    }
                }
                /* Should never reach here for valid n */
                return 0;
            }
        }

        cum += evens;   /* move to the next row */
    }
}
