/*
 * A138776  Triangle read by rows: T(n,k)=binomial(n-2k,3k+1) (n>=1,
 *          0<=k<=(n-1)/5).
 * https://oeis.org/A138776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r-2k, 3k+1), where (r,k) are the * row and
 *            column indices of the triangular array T(r,k) = binomial(r-2k,
 *            3k+1). * The sequence is read row‑by‑row; for a given n ≥ 1: * -
 *            determine row r such that the cumulative number of terms before
 *            * row r is less than n and including row r is...
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  // offset behavior is undefined; return 0 for safety

    idx_t r = 1;
    idx_t sum = 0;
    idx_t len;

    /* Find the row r that contains the n-th term.  Length of row r is
     * ceil(r/5) = (r+4)/5. */
    while (1) {
        len = (r + 4) / 5;          /* number of terms in row r */
        if (sum + len >= n)
            break;
        sum += len;
        ++r;
    }

    /* k is the zero‑based offset within the row. */
    idx_t k = n - sum - 1;

    /* Compute the element: binomial(r - 2k, 3k + 1). */
    return binomial(r - 2 * k, 3 * k + 1);
}
