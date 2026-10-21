/*
 * A208233  First inverse function (numbers of rows) for pairing function
 *          A188568.
 * https://oeis.org/A208233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is defined by let t = floor((-1+sqrt(8*n-7))/2) let i = n -
 *            t*(t+1)/2 let j = (t*t+3*t+4)/2 - n then a(n) = max(i,j) if
 *            (i>=j && i even) or (i<j && j odd) a(n) = min(i,j) otherwise
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find minimal m such that m*(m+1)/2 >= n; then t = m-1. */
    idx_t low = 0, high = n;   /* high is safe because n*(n+1)/2 >= n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri_mid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low - 1;   /* t such that t*(t+1)/2 < n <= (t+1)*(t+2)/2 */

    /* Compute i and j */
    unsigned __int128 tri_t = (unsigned __int128)t * (t + 1) / 2;
    idx_t i = n - (idx_t)tri_t;

    unsigned __int128 tri_t2 = ((unsigned __int128)t * t
                               + (unsigned __int128)3 * t
                               + 4) / 2;
    idx_t j = (idx_t)tri_t2 - n;

    /* Determine the result */
    idx_t min_v = (i < j) ? i : j;
    idx_t max_v = (i < j) ? j : i;
    idx_t ans;

    if (i >= j) {
        ans = (i & 1) ? min_v : max_v;   /* i even -> max, i odd -> min */
    } else {
        ans = (j & 1) ? max_v : min_v;   /* j odd  -> max, j even -> min */
    }

    return (seq_t)ans;
}
