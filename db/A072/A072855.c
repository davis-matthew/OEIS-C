/*
 * A072855  Number of permutations satisfying i-3<=p(i)<=i+5, i=1..n.
 * https://oeis.org/A072855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the number of permutations p of 1..n such that
   for every i (1 ≤ i ≤ n):  i-3 ≤ p(i) ≤ i+5.
   This is a banded assignment problem.  We process positions from 1
   to n, keeping a 9‑bit mask that records which values inside the
   current window [i-3, i+5] have not yet been used.  For each
   available value we assign it and shift the window one step to the
   right, adding the new value i+6 if it is ≤ n.  The masks are at most
   512 (9 bits), so the algorithm runs in O(n·512) time.  All
   computations use type seq_t. */

seq_t a(idx_t n)
{
    const int WIDTH   = 9;
    const int MASKS   = 1 << WIDTH;   /* 512 */
    seq_t dp[MASKS], ndp[MASKS];
    int i, bit, mask, bits;

    /* initialise dp array */
    for (i = 0; i < MASKS; ++i) dp[i] = 0;

    /* initial window for position i = 1:  values from -2 to 6 */
    int init_mask = 0;
    for (bit = 0; bit < WIDTH; ++bit) {
        idx_t val = (idx_t)(-2 + bit);          /* 1 - 3 + bit */
        if (val >= 1 && val <= n)
            init_mask |= (1 << bit);
    }
    dp[init_mask] = 1;

    /* process positions 1 .. n-1 */
    for (i = 1; i < n; ++i) {
        /* clear next dp */
        for (mask = 0; mask < MASKS; ++mask) ndp[mask] = 0;
        idx_t new_right = i + 6;                /* value entering the right edge */

        for (mask = 0; mask < MASKS; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            bits = mask;
            while (bits) {
                int lb   = bits & -bits;          /* lowest set bit */
                bit      = __builtin_ctz(lb);      /* its index */
                bits ^= lb;                       /* clear it */

                /* remove the chosen value and shift the window right */
                int new_mask = (mask & ~(1 << bit)) >> 1;

                /* add new value entering the window, if it is valid */
                if (new_right <= n)
                    new_mask |= (1 << (WIDTH - 1));

                ndp[new_mask] += ways;
            }
        }

        /* swap dp and ndp */
        for (mask = 0; mask < MASKS; ++mask) dp[mask] = ndp[mask];
    }

    /* final position i = n: we must exhaust all remaining values */
    seq_t result = 0;
    for (mask = 0; mask < MASKS; ++mask) {
        seq_t ways = dp[mask];
        if (!ways) continue;

        bits = mask;
        while (bits) {
            int lb   = bits & -bits;
            bit      = __builtin_ctz(lb);
            bits ^= lb;

            int new_mask = mask & ~(1 << bit);   /* no shift after last position */
            if (new_mask == 0)
                result += ways;
        }
    }

    return result;
}
