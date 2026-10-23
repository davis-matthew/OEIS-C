/*
 * A175465  a(n) is the number of elements in the largest set of same-sized
 *          run-lengths (of either 0's or 1's) in the binary representation of
 *          n.
 * https://oeis.org/A175465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = maximum number of runs of the same length among runs of
 *            0s or 1s in the binary representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* There are at most 64 runs (since n fits in a 64‑bit integer). */
    seq_t run_counts[65] = {0};
    idx_t temp = n;
    int prev_bit = -1;    /* no previous bit yet */
    int run_len  = 0;     /* current run length */

    /* Scan the bits of n from least‑significant to most‑significant. */
    while (temp != 0)
    {
        int bit = (int)(temp & 1);   /* current bit (0 or 1) */
        if (bit == prev_bit)
        {
            run_len++;              /* extend current run */
        }
        else
        {
            if (prev_bit != -1)                 /* finish previous run */
                run_counts[run_len]++;          /* count it */
            prev_bit = bit;                    /* start new run */
            run_len  = 1;
        }
        temp >>= 1;
    }

    /* Record the final run. */
    if (prev_bit != -1)
        run_counts[run_len]++;

    /* Find the largest count among all run lengths. */
    seq_t maxcnt = 0;
    for (int i = 1; i < 65; ++i)
        if (run_counts[i] > maxcnt)
            maxcnt = run_counts[i];

    return maxcnt;
}
