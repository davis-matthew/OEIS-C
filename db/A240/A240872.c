/*
 * A240872  Number of partitions p of n into distinct parts such that max(p) =
 *          4 + min(p).
 * https://oeis.org/A240872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count partitions of n into distinct parts such that max = min + 4.
   For each possible minimal part m (m ≥ 1) the maximal part must be m+4.
   The remaining parts can be any subset of {m+1, m+2, m+3}.  Each such
   subset yields a distinct partition.  The function iterates over all
   m with 2·m + 4 ≤ n, checks whether the sum needed to reach n can be
   formed from the subset, and counts the valid partitions.
   Time complexity: O(n).  Works for any indexed n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t m = 1; ; ++m)
    {
        idx_t base = 2 * m + 4;            /* sum of mandatory parts m and m+4 */
        if (base > n) break;               /* no larger m can work */

        idx_t S = n - base;                /* sum required from  {m+1,m+2,m+3} */
        if (S > 3 * m + 6) continue;       /* even full subset too small */

        /* Enumerate all 8 subsets of {m+1, m+2, m+3} */
        if (S == 0          ) cnt++;        /* empty set   -> {m, m+4}          */
        if (S == (m + 1)    ) cnt++;        /* {m+1}       -> {m, m+1, m+4}     */
        if (S == (m + 2)    ) cnt++;        /* {m+2}       -> {m, m+2, m+4}     */
        if (S == (m + 3)    ) cnt++;        /* {m+3}       -> {m, m+3, m+4}     */
        if (S == (2 * m + 3)) cnt++;        /* {m+1,m+2}   -> {m, m+1, m+2, m+4 }*/
        if (S == (2 * m + 4)) cnt++;        /* {m+1,m+3}   -> {m, m+1, m+3, m+4}*/
        if (S == (2 * m + 5)) cnt++;        /* {m+2,m+3}   -> {m, m+2, m+3, m+4}*/
        if (S == (3 * m + 6)) cnt++;        /* {m+1,m+2,m+3} -> {m, m+1, m+2, m+3, m+4} */
    }
    return cnt;
}
