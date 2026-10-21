/*
 * A057944  Largest triangular number less than or equal to n; write m-th
 *          triangular number m+1 times.
 * https://oeis.org/A057944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = floor((sqrt(1+8*n)-1)/2)*floor((sqrt(1+8*n)+1)/2)/2 (i.e.
 *            a(n) = T_t where t is the largest integer with t*(t+1)/2 ≤ n)
 * Range:     u8 n <= 275, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find t = floor((sqrt(1+8*n)-1)/2) using a binary search
       without any floating‑point operations. */
    idx_t low  = 0;
    idx_t high = n;                  /* t ≤ n for all n ≥ 0 */

    while (low < high)
    {
        /* mid = ceil((low+high)/2) to avoid infinite loop when low+1==high */
        unsigned __int128 span = (unsigned __int128)high - low + 1;
        idx_t mid = low + (span >> 1);           /* mid in [low, high] */

        /* Condition: mid*(mid+1)/2 ≤ n  ⇔  mid*(mid+1) ≤ 2*n   */
        unsigned __int128 left  = (unsigned __int128)mid * (mid + 1);
        unsigned __int128 right = (unsigned __int128)n * 2;

        if (left <= right)
            low = mid;          /* mid is feasible, search higher */
        else
            high = mid - 1;     /* mid too large, search lower */
    }

    /* low is now the desired t */
    idx_t t = low;
    unsigned __int128 triangular = (unsigned __int128)t * (t + 1) / 2;
    return (seq_t)triangular;
}
