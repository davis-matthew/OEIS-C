/*
 * A117965  Triangle read by rows: T(i,j) = F(i)*F(j)*C(i,j) for 1 <= j <= i,
 *          where F(n) is the n-th Fibonacci number and C(n,m) is a binomial
 *          coefficient.
 * https://oeis.org/A117965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(i) * F(j) * C(i,j) where i is the minimal row with *
 *            triangular(i) >= n and j = n - triangular(i-1)
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, fib, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include "../utils/triangular.h"
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row i such that triangular(i) >= n. */
    idx_t low = 1, high = n;          /* n is always >= 1 */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;

    /* Determine column j within that row. */
    idx_t j = n - triangular(i - 1);

    /* Compute the term: F(i) * F(j) * C(i, j). */
    seq_t Fi = fib(i);
    seq_t Fj = fib(j);
    seq_t bin = binomial(i, j);

    return (Fi * Fj) * bin;
}
