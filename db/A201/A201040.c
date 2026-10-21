/*
 * A201040  Number of -6..6 arrays of n elements with adjacent element
 *          differences also in -6..6.
 * https://oeis.org/A201040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n over * the alphabet {-6,…,6}
 *            such that successive differences * belong to {-6,…,6}. It is
 *            computed by a single * O(n) dynamic program over the 13 possible
 *            values. * The DP coincides with the empirical recurrence *
 *            a(n)=10 a(n‑1)+3 a(n‑2)-18 a(n‑3)-a(n‑4)+8 a(n‑...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Quick return for n==0 (not part of the official
     * sequence but harmless to handle).  */
    if (n == 0)
        return 0;

    /* State arrays: cur[i] holds the number of length‑k
     * arrays ending with value (i‑6).  For k=1 every value is
     * reachable exactly once. */
    seq_t cur[13];
    seq_t nxt[13];
    seq_t pref[13];

    /* initialise for k = 1 */
    for (int i = 0; i < 13; ++i)
        cur[i] = (seq_t)1;

    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 13; ++i)
            total += cur[i];
        return total;
    }

    /* iterate for lengths 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        /* build prefix sums of cur for fast range sum */
        pref[0] = cur[0];
        for (int i = 1; i < 13; ++i)
            pref[i] = pref[i-1] + cur[i];

        /* compute next state: for each target value j
         * sum cur over all sources i with |i-j| ≤ 6 */
        for (int j = 0; j < 13; ++j) {
            int l = j - 6;
            if (l < 0) l = 0;
            int r = j + 6;
            if (r > 12) r = 12;
            nxt[j] = pref[r] - (l > 0 ? pref[l-1] :  (seq_t)0);
        }

        /* move to next iteration */
        for (int i = 0; i < 13; ++i)
            cur[i] = nxt[i];
    }

    /* total number of valid arrays of length n */
    seq_t total = 0;
    for (int i = 0; i < 13; ++i)
        total += cur[i];
    return total;
}
