/*
 * A045975  Take the first odd integer and multiple of 1, the next 2 even
 *          integers and multiples of 2, the next 3 odd integers and multiples
 *          of 3, the next 4 even integers and multiples of 4, ...
 * https://oeis.org/A045975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) = k * m * where k is the smallest integer with k*(k+1)/2 >=
 *            n, * i = n - k*(k-1)/2, * m = k*(k-1)/2 + (i-1) for even k, * m
 *            = (k-1)^2/2 + 1 + 2*(i-1) for odd k. * All arithmetic performed
 *            in seq_t
 * Range:     u8 n <= 32, u16 n <= 1293, u32 n <= 9809, u64 n <= 9809,
 *            u128 n <= 9809, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* Offset starts at 1 */
    idx_t lo = 0, hi = 1;
    /* Find an upper bound for k */
    while (( (__int128)hi * (hi + 1) ) / 2 < n)
    {
        lo = hi;
        hi <<= 1;
    }
    /* Binary search for the minimal k with k*(k+1)/2 >= n */
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) ) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t k = hi;
    idx_t i = n - ( ( (__int128)k * (k - 1) ) / 2 );    /* 1‑based position inside row */
    seq_t k_s = (seq_t)k;
    seq_t k1_s = (seq_t)(k - 1);
    seq_t m;
    if ((k & 1) == 0)            /* even k */
    {
        m = k1_s * k_s / 2 + (seq_t)(i - 1);
    }
    else                         /* odd k */
    {
        m = k1_s * k1_s / 2 + (seq_t)1 + (seq_t)(i - 1) * (seq_t)2;
    }
    return k_s * m;
}
