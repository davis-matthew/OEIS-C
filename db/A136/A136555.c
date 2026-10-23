/*
 * A136555  Square array, read by antidiagonals, where T(n,k) = binomial(2^k +
 *          n-1, k).
 * https://oeis.org/A136555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the square array T(i,j) = binomial(2^j + i - 1,
 *            j) read by antidiagonals (j goes from 0 up to i+j on each
 *            diagonal)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 34, u64 n <= 64, u128 n <= 103,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index s (i+j) that contains the linear index n. */
    idx_t s   = 0;     /* current diagonal sum i+j */
    idx_t cum = 0;     /* number of elements before this diagonal */

    while (1) {
        idx_t diag_len = s + 1;          /* length of current diagonal */
        if (cum + diag_len > n) break;   /* n lies on this diagonal */
        cum += diag_len;
        ++s;
    }

    /* Position of n within this diagonal. */
    idx_t offset = n - cum;          /* 0 <= offset <= s */
    idx_t i      = s - offset;       /* row index */
    idx_t j      = offset;           /* column index (k) */

    /* T(i,0) is always 1. */
    if (j == 0) return 1;

    /* Compute the binomial coefficient binomial(2^j + i - 1, j). */
    seq_t n_val = pow2(j) + (seq_t)i - 1;   /* 2^j + i - 1 as seq_t */

    seq_t res = 1;
    for (idx_t r = 1; r <= j; ++r) {
        /* numerator: n_val - (r - 1) */
        seq_t numer = n_val - (seq_t)(r - 1);
        seq_t denom = (seq_t)r;

        /* cancel common factors between res and denom */
        seq_t g1 = gcd(res, denom);
        res /= g1;
        denom /= g1;

        /* cancel remaining factors between numer and denom */
        seq_t g2 = gcd(numer, denom);
        numer /= g2;
        denom /= g2;

        /* After cancellations denom should be 1. */
        res *= numer;
    }

    return res;
}
