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
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   binomial, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper utilities */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/triangular.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

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
