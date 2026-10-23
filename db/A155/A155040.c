/*
 * A155040  A symmetric (1,-1)-triangle.
 * https://oeis.org/A155040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 1 if min(k,r‑k) is even * -1 otherwise * * where r is the
 *            row index with r(r+1)/2 ≤ n < (r+1)(r+2)/2 * and k = n –
 *            r(r+1)/2 (0‑based column)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find r by binary search using 128‑bit arithmetic. */
    idx_t low = 0;
    idx_t high = 1;

    /* Grow high until the triangular number exceeds n. */
    while ( ((__uint128_t)high * (high + 1) / 2) <= n )
        high <<= 1;

    /* Binary search for the largest r with r(r+1)/2 ≤ n. */
    while ( low + 1 < high )
    {
        idx_t mid = low + ((high - low) >> 1);
        if ( ((__uint128_t)mid * (mid + 1) / 2) <= n )
            low = mid;
        else
            high = mid;
    }

    idx_t r = low;   /* r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 */

    /* Compute column k. */
    idx_t prev = (idx_t)( (__uint128_t)r * (r + 1) / 2 );
    idx_t k = n - prev;

    /* Parity of min(k, r-k). */
    idx_t m = (k <= r - k) ? k : r - k;
    if ( (m & 1) == 0 )
        return (seq_t)1;
    else
        return (seq_t)-1;
}
