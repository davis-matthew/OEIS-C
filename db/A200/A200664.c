/*
 * A200664  Number of 0..4 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its three previous neighbors modulo 5.
 * https://oeis.org/A200664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays [0..4] of length n such that for each
 *            index i (with x[j] = 0 for j < 0), x[i] ≥ (x[i‑1] + x[i‑2] +
 *            x[i‑3]) mod 5. Computed by dynamic programming over the last
 *            three values
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* not defined for offset 0 */
    }

    /* 5³ = 125 possible triples (a,b,c) with a,b,c in 0..4.
       State index = a*25 + b*5 + c. */
    static seq_t buf1[125], buf2[125];
    seq_t *cur = buf1, *nxt = buf2;

    /* initialise current DP state: start from triple (0,0,0) with count 1 */
    for (int i = 0; i < 125; ++i)
        cur[i] = 0;
    cur[0] = 1;

    /* iterate over positions 1 .. n */
    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next array */
        for (int i = 0; i < 125; ++i)
            nxt[i] = 0;

        /* transition */
        for (int s = 0; s < 125; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;

            int a = s / 25;          /* most significant */
            int b = (s / 5) % 5;     /* middle   */
            int c = s % 5;           /* least    */

            int sum = (a + b + c) % 5;

            /* allowed next value d satisfies d ≥ sum (mod 5) */
            for (int d = sum; d <= 4; ++d) {
                int t = b * 25 + c * 5 + d;   /* new state */
                nxt[t] += val;
            }
        }

        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* total number of arrays is sum over all end states */
    seq_t total = 0;
    for (int i = 0; i < 125; ++i)
        total += cur[i];

    return total;
}
