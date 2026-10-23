/*
 * A168641  Triangle read by rows: T(n,k) = [x^k] p(x,n), where p(x,n) = 3*(x
 *          + 1)^n - 2*(x^n + 1) - n*(x + x^(n - 1)) for n >= 2, p(x,0) = 1,
 *          and p(x,1) = x + 1.
 * https://oeis.org/A168641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, tabl
 * Formula:   a(n) = flattening of triangle T, where T(n,k) = [x^k] p(x,n)
 *            with p(x,n) = 3*(x+1)^n - 2*(x^n+1) - n*(x + x^(n-1)) for n≥2,
 *            (p(x,0)=1, p(x,1)=x+1). The sequence is a(n)=T(r,k) where r =
 *            row index and k = column index obtained from n by interpreting
 *            the triangle row–by–row
 * Range:     u8 n <= 39, u16 n <= 159, u32 n <= 608, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 is handled by the general method below,
       but we keep it explicit for clarity. */
    if (n == 0) return (seq_t)1;

    /* Determine row index r such that offset(r) ≤ n < offset(r+1),
       where offset(r) = r*(r+1)/2.  This is the floor of
       (sqrt(1+8n)-1)/2.  int_sqrt returns the floor of the square root. */
    idx_t s = int_sqrt((idx_t)1 + (idx_t)8 * n);
    idx_t r = (s - (idx_t)1) / (idx_t)2;

    idx_t offset = r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = n - offset;                /* column within row r */

    /* Boundary entries of the triangle are always 1. */
    if (k == (idx_t)0 || k == r) return (seq_t)1;

    /* Handle the special case r = 2, k = 1. */
    if (r == (idx_t)2 && k == (idx_t)1) return (seq_t)2;

    /* For r ≥ 3, the first and last interior entries equal 2*r. */
    if (k == (idx_t)1 || k == r - (idx_t)1) return (seq_t)(2 * r);

    /* Remaining entries: T(r,k) = 3 * binomial(r,k). */
    seq_t val = binomial(r, k);
    val = val * ((seq_t)3);
    return val;
}
