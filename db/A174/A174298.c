/*
 * A174298  Triangle T(n, k) = binomial(n, k)*( n!/k! if floor(n/2) >= k
 *          otherwise n!/(n-k)! ), read by rows.
 * https://oeis.org/A174298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial(n,k)^2 * ( (n-k)! if floor(n/2) >= k else k!
 *            ). The sequence a(n) is the linear read of the triangular array
 *            T (row 0 first element, then row 1, row 2, ...)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 192,
 *            u128 n <= 531, big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = (idx_t)sqrt((double)(2 * (double)n));
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    /* Compute offset and column index k. */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;

    /* Compute T(r,k). */
    seq_t bin = binom(r, k);
    seq_t sq = bin * bin;
    seq_t fact;
    if (r / 2 >= k) {
        fact = factorial(r - k);
    } else {
        fact = factorial(k);
    }
    return sq * fact;
}
