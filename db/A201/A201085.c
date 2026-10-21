/*
 * A201085  Number of -5..5 arrays of n elements with first and second
 *          differences also in -5..5.
 * https://oeis.org/A201085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence: a(1)=11; for n≥2 a(n) is the number of sequences of length n
    with values in [-5,5] whose first and second differences are also in
    [-5,5].  The implementation performs a dynamic programming on the last
    two values.  For each pair (x_{i-1},x_i) it keeps the count of
    sequences ending with this pair and extends them by all admissible
    next values.  The algorithm runs in O(n·11³) time and uses only 121
    state variables at any instant. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* offset 1, undefined for n=0 */
    if (n == 1)
        return (seq_t)11;      /* any of the 11 values */

    /* DP tables for states (prev2,prev1) */
    seq_t cur[11][11];
    seq_t next[11][11];

    /* initialise for n = 2: all pairs with first difference in [-5,5] */
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j)
            cur[i][j] = 0;

    for (int a1 = 0; a1 < 11; ++a1) {
        int val_a1 = a1 - 5;
        for (int a2 = 0; a2 < 11; ++a2) {
            int val_a2 = a2 - 5;
            int diff1 = val_a2 - val_a1;            /* first difference */
            if (diff1 >= -5 && diff1 <= 5)
                cur[a1][a2] = 1;
        }
    }

    if (n == 2) {
        seq_t total = 0;
        for (int i = 0; i < 11; ++i)
            for (int j = 0; j < 11; ++j)
                total += cur[i][j];
        return total;
    }

    /* extend sequences up to length n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next table */
        for (int i = 0; i < 11; ++i)
            for (int j = 0; j < 11; ++j)
                next[i][j] = 0;

        for (int prev2 = 0; prev2 < 11; ++prev2) {
            int val_prev2 = prev2 - 5;
            for (int prev1 = 0; prev1 < 11; ++prev1) {
                seq_t cnt = cur[prev2][prev1];
                if (!cnt)
                    continue;
                int val_prev1 = prev1 - 5;
                for (int x = 0; x < 11; ++x) {
                    int val = x - 5;
                    int diff1 = val - val_prev1;              /* first diff */
                    if (diff1 < -5 || diff1 > 5)
                        continue;
                    int diff2 = val - 2 * val_prev1 + val_prev2; /* second diff */
                    if (diff2 < -5 || diff2 > 5)
                        continue;
                    next[prev1][x] += cnt;
                }
            }
        }

        /* copy next into cur for next iteration */
        for (int i = 0; i < 11; ++i)
            for (int j = 0; j < 11; ++j)
                cur[i][j] = next[i][j];
    }

    /* sum all sequences of length n */
    seq_t total = 0;
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j)
            total += cur[i][j];

    return total;
}
