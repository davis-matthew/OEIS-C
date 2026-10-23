/*
 * A156864  Triangle read by rows: T(n, k) = 2^k - binomial(n+1, k+1) -
 *          ((2*k-n)/(k+1)) * binomial(n+1, k).
 * https://oeis.org/A156864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is T(r,k) where r is the row with a(1)=0, T(n,k) = 2^k -
 *            binomial(n+1,k+1) - ((2*k-n)/(k+1)) * binomial(n+1,k)
 * Range:     i8 n <= 22, i16 n <= 135, i32 n <= 527, i64 n <= 2079,
 *            i128 n <= 5050, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n (rows start at 1). */
    idx_t r = 1;
    idx_t tri = 1;                     /* current triangular number */
    while (tri < n)
    {
        ++r;
        tri += r;
    }

    idx_t offset = (r - 1) * r / 2;     /* first index of row r */
    idx_t k = n - offset;              /* column index within the row (1‑based) */

    seq_t pow2k  = pow2(k);
    seq_t binom1 = binomial(r + 1, k + 1); /* binomial(n+1, k+1) */
    seq_t binom2 = binomial(r + 1, k);     /* binomial(n+1, k)   */

    /* Compute ((2*k-n)/(k+1)) * binomial(n+1,k) exactly. */
    seq_t term_num = binom2 * (((seq_t)2 * (seq_t)k) - (seq_t)r);
    seq_t term_den = (seq_t)(k + 1);
    seq_t part3 = term_num / term_den;

    return pow2k - binom1 - part3;
}
