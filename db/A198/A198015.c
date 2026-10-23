/*
 * A198015  Number of isomorphism classes of nanocones with 4 pentagons and a
 *          symmetric boundary of length n.
 * https://oeis.org/A198015
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the rational generating function A(x) =
 *            x^2*(-x^11 + x^10 - x^9 + 4*x^8 + 5*x^7 + 10*x^6 + 9*x^5 +
 *            11*x^4 + 4*x^3 + 7*x^2 + 2*x + 1) / ((x - 1)^6*(x + 1)^3*(x^2 +
 *            1)*(x^2 + x + 1)). The coefficients satisfy a linear recurrence
 *            of order 13 derived from the denominator
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const long long D[14] = {
    // D[0] through D[13], coefficients of (x-1)^6*(x+1)^3*(x^2+1)*(x^2+x+1)
    1,   -2,  -1,   3,  0,   1,  -2,  -2,   1,  0,   3,  -1,  -2,   1
};

static const long long B[14] = {
    // B[0] through B[13], coefficients of the numerator N(x) = x^2 * p(x)
    //             p(x) = -x^11 + x^10 - x^9 + 4*x^8 + 5*x^7 + 10*x^6 + 9*x^5
    //                   + 11*x^4 + 4*x^3 + 7*x^2 + 2*x + 1
    0, 0, 1, 2, 7, 4, 11, 9, 10, 5, 4, -1, 1, -1
};

seq_t a(idx_t n)
{
    /* Cache the sequence values to handle out-of-order queries. */
    static seq_t *seq = NULL;
    static idx_t  cap   = 0;
    static idx_t  len   = 0;

    if (n < 2)                    /* a(0) and a(1) are undefined in OEIS */
        return (seq_t)0;

    /* Grow the cache if needed. */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed */
            /* In a real application you might handle the error differently. */
            abort();
        }
        seq = tmp;
        cap = new_cap;
    }

    /* Initialise first two terms if this is the first call. */
    if (len == 0) {
        seq[0] = 0;
        seq[1] = 0;
        len   = 2;
    }

    /* Compute terms up to n.  The recurrence is
          a[n] = B[n] + Σ_{k=1..13} (if D[k] < 0) |D[k]|*a[n-k]
                                       else -D[k]*a[n-k]
    */
    for (idx_t i = len; i <= n; ++i) {
        long long val = 0;
        if (i < 14)                 /* B[i] may be zero for i >=14 */
            val = B[i];

        for (int k = 1; k <= 13; ++k) {
            if ((idx_t)k > i)
                break;
            long long dk = D[k];
            if (dk > 0) {
                val -= dk * (long long)seq[i - k];
            } else if (dk < 0) {
                val += (-dk) * (long long)seq[i - k];
            }
        }
        seq[i] = (seq_t)val;          /* val is guaranteed to be non‑negative */
    }

    len = n + 1;
    return seq[n];
}
