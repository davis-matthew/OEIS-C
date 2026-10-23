/*
 * A144431  Triangle read by rows: T(n,k) (1 <= k <= n) given by T(n,1) =
 *          T(n,n) = 1, otherwise T(n, k) = (m*n-m*k+1)*T(n-1,k-1) +
 *          (m*k-m+1)*T(n-1,k), where m = -1.
 * https://oeis.org/A144431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) is the value in the triangle T(n,k) when the rows are read
 *            * one after another. T(r,k) is defined by the recurrence *
 *            T(r,1) = T(r,r) = 1 * T(r,k) = (-r + k + 1) * T(r-1,k-1) + (2 -
 *            k) * T(r-1,k) (m = -1) * A closed form for r ≥ 3 is * T(r,k) =
 *            (-1)^(k-1) * C(r-3,k-1) + (-1)^(r+k) * C(r-3,k-...
 * Range:     i8 n <= 81, i16 n <= 217, i32 n <= 681, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/*---------- helper: row that contains the flat index n (1‑based) ----------*/
static idx_t _row_for_index(idx_t n)
{
    /* solve r(r+1)/2 ≥ n */
    double d = sqrt(1.0 + 8.0 * (double)n);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* adjust for rounding errors */
    while (((__int128)r * (r + 1) / 2) < n) ++r;
    while (r > 0 && (((__int128)(r - 1) * r) / 2) >= n) --r;
    return r;
}

/*---------- helper: binomial coefficient C(N,K) (N≥0, 0≤K≤N) -------------*/
static seq_t _binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;               /* use symmetry */
    __int128 res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        res = res * (N - K + i);            /* multiply then divide */
        res = res / i;
    }
    return (seq_t)(long long)res;           /* conversion safe for values that fit */
}

/*--------------------------- main sequence function -------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined but safe */

    /* locate the row and position within that row */
    idx_t r = _row_for_index(n);           /* current row number 1‑based */
    idx_t prev_rows = ((__int128)(r - 1) * r) / 2;   /* entries before row r */
    idx_t k = n - prev_rows;                  /* position in row r (1‑based) */

    /* rows 1 and 2 consist entirely of 1’s */
    if (r <= 2) return 1;

    /* use the closed–form expression for r ≥ 3 */
    seq_t sign1 = ((k - 1) & 1) ? - (seq_t)1 : (seq_t)1;
    seq_t term1 = sign1 * _binom(r - 3, k - 1);

    seq_t term2 = 0;
    if (k >= 3)
    {
        seq_t sign2 = (((r + k) & 1) ? - (seq_t)1 : (seq_t)1);
        term2 = sign2 * _binom(r - 3, k - 3);
    }

    return term1 + term2;
}
