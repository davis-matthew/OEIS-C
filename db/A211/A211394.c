/*
 * A211394  T(n,k) = (k+n)*(k+n-1)/2-(k+n-1)*(-1)^(k+n)-k+2; n , k > 0, read
 *          by antidiagonals.
 * https://oeis.org/A211394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+n)*(k+n-1)/2 - (k+n-1)*(-1)^(k+n) - k + 2 where (n,k)
 *            are obtained by reading the (k,n)‐table along antidiagonals:
 *            (1,1), (1,2),(2,1),(1,3),(2,2),(3,1), …
 * Range:     u8 n <= 232, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute T(n,k) from the formula --------------------------------------------------*/
static inline seq_t T(idx_t row, idx_t col)
{
    seq_t sum = (seq_t)(row + col);          /* k+n */
    seq_t base = sum * (sum - 1) / 2;        /* (k+n)(k+n-1)/2 */

    /* subtract (k+n-1)*(-1)^(k+n) */
    /* if sum is even (-1)^sum = 1  →  base - (sum-1)
       if sum is odd  (-1)^sum = -1 →  base + (sum-1)  */
    seq_t part;
    if ((sum & 1) == 0)
        part = base - (sum - 1);
    else
        part = base + (sum - 1);

    /* minus k + 2  (k is the column index) */
    return part - (seq_t)col + 2;
}

/* map the linear index n to (row,col) by antidiagonal enumeration */
static inline void index_to_rc(idx_t n, idx_t *row, idx_t *col)
{
    /* find smallest t such that t*(t-1)/2 >= n   (t >= 2) */
    idx_t lo = 2, hi = 2;
    /* increase hi until condition holds, avoiding overflow with __int128 */
    while ((unsigned __int128)hi * (hi-1)/2 < n)
        hi <<= 1;

    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 cum = (unsigned __int128)mid * (mid-1) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t t = lo;                                   /* diagonal number (sum)   */
    unsigned __int128 cum_prev = (unsigned __int128)(t-1) * (t-2) / 2;  /* rows before this diagonal */
    idx_t offset = (idx_t)(n - cum_prev - 1);       /* 0‑based offset inside diagonal */
    *row = offset + 1;                              /* rows are 1‑based */
    *col = t - *row;
}

/* external function required by the build ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)   /* out of required range; behaviour arbitrary */
        return 0;

    idx_t row, col;
    index_to_rc(n, &row, &col);
    return T(row, col);
}
