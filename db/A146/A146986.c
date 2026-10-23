/*
 * A146986  Triangle, read by rows, T(n, k) = binomial(n, k) for n < 2 and
 *          binomial(n, k) + 2^(n-1) * binomial(n-2, k-1) otherwise.
 * https://oeis.org/A146986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 69, u32 n <= 197, u64 n <= 646,
 *            u128 n <= 2373, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = binomial(n,k) for n<2,
   otherwise T(n,k)=binomial(n,k)+2^(n-1)*binomial(n-2,k-1).
   a(n) is the n-th entry of the triangle, row-major. */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    /* find the first r such that r*(r+1)/2 > n */
    while ( ((unsigned __int128)r * (r + 1) / 2) <= n ) {
        ++r;
    }
    --r; /* now r is the row index */

    idx_t sum_prev = r * (r + 1) / 2;      /* sum of elements in rows before r */
    idx_t col = n - sum_prev;              /* column within the row */

    seq_t val = binom(r, col);             /* base binomial term */

    if (r >= 2 && col > 0) {
        seq_t add = pow2(r - 1) * binom(r - 2, col - 1);
        val += add;
    }

    return val;
}
