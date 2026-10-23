/*
 * A140978  Repeat (n+1)^2 n times.
 * https://oeis.org/A140978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (k+1)^2 where k is the smallest integer such that
 *            k*(k+1)/2 >= n (i.e. k = ceil( (sqrt(8*n+1)-1)/2 ) )
 * Range:     u8 n <= 105, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the minimal m such that m*(m+1)/2 >= n. */
    idx_t low = 1, high = 1;
    /* Expand high until it satisfies the inequality. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t m = low;                   /* m >= 1 for n >= 1 */
    seq_t v = (seq_t)(m + 1);        /* (m+1) fits safely into seq_t */
    return v * v;                    /* (m+1)^2 */
}
