/*
 * A211439  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w+3x+3y=0.
 * https://oeis.org/A211439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y ∈ [-n,…,n]
 *            and w + 3x + 3y = 0
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to signed type for easy handling of negative sums. */
    long long N = (long long)n;
    /* s = x + y must satisfy -⌊N/3⌋ ≤ s ≤ ⌊N/3⌋.  */
    long long s_min = -(N / 3);   /* ceil(-N/3) for non‑negative N */
    long long s_max =  N / 3;     /* floor(N/3)       */

    seq_t result = 0;
    for (long long s = s_min; s <= s_max; ++s)
    {
        /* Count pairs (x,y) with x,y ∈ [-N,N] and x + y = s.  */
        long long low  = s - N;          /* from y = s - x ≥ -N   */
        if (low < -N) low = -N;          /* enforce x ≥ -N   */
        long long high = s + N;          /* from y = s - x ≤ N   */
        if (high > N) high = N;          /* enforce x ≤  N   */
        if (high >= low)
        {
            seq_t cnt = (seq_t)(high - low + 1);
            result += cnt;
        }
    }
    return result;
}
