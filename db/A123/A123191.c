/*
 * A123191  Triangle read by rows: T(n,k) is the coefficient of x^k in the
 *          polynomial P[n] defined by P[0]=1, P[1]=x-1,
 *          P[n]=(1-x)P[n-1]+xP[n-2] for n>=2. Alternatively,
 *          P[n]=-1-(-x)^n-3*Sum((-x)^k,k=1..n-1).
 * https://oeis.org/A123191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows * defined
 *            by * T(0,0) = 1 * For n ≥ 1: * T(n,0) = –1 * T(n,n) = (–1)^(n+1)
 *            * T(n,k) = (–1)^(k+1) * 3 for 1 ≤ k ≤ n–1 * * The sequence a(n)
 *            is obtained by listing the triangle coefficients * in row-major
 *            order
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle first element explicitly */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r such that
     *   r(r+1)/2 ≤ n < (r+1)(r+2)/2
     *   total terms up to row r-1 is r(r+1)/2
     *   total terms up to row r   is (r+1)(r+2)/2
     */
    idx_t r = 0;
    idx_t total = 1;                /* total terms up to current row r */
    while (total <= n) {
        r++;
        total += r + 1;             /* add length of next row */
    }

    /* total now equals (r+1)(r+2)/2, and r is the required row */
    idx_t previous_total = total - (r + 1); /* terms up to row r-1 */
    idx_t c = n - previous_total;          /* column index (0‑based) */

    /* Compute the coefficient in the triangle */
    if (c == 0) {
        /* T(r,0) = -1 for r ≥ 1 */
        return (seq_t)-1;
    } else if (c == r) {
        /* T(r,r) = (–1)^(r+1) */
        return (((r + 1) & 1) ? (seq_t)-1 : (seq_t)1);
    } else {
        /* 1 ≤ c ≤ r–1: T(r,c) = (–1)^(c+1)*3 */
        return ((((c + 1) & 1) ? (seq_t)-3 : (seq_t)3));
    }
}
