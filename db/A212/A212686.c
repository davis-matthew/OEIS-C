/*
 * A212686  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          2|w-x|=n+|y-z|.
 * https://oeis.org/A212686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered quadruples (w,x,y,z) with w,x,y,z ∈
 *            {1,…,n} satisfying 2·|w‑x| = n + |y‑z|
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases 0 and 1 */
    if (n <= 1)
        return 0;

    seq_t total = 0;

    /* t = |y‑z| ranges from 0 to n‑1.
       Only when n and t have the same parity does (n+t)/2 become integer. */
    for (idx_t t = 0; t < n; ++t)
    {
        if (((n ^ t) & 1) != 0)   /* parity mismatch */
            continue;

        /* s = |w‑x| = (n + t)/2  */
        seq_t s = (seq_t)(n + t) / 2;

        /* Count ordered pairs (y,z) with |y‑z| = t */
        seq_t count_yz = (t == 0) ? (seq_t)n : 2 * ((seq_t)n - (seq_t)t);

        /* Count ordered pairs (w,x) with |w‑x| = s */
        seq_t count_wx = (s == 0) ? (seq_t)n : 2 * ((seq_t)n - s);

        total += count_yz * count_wx;
    }

    return total;
}
