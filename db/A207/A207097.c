/*
 * A207097  Number of 0..5 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 6.
 * https://oeis.org/A207097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 6, a(2) = 21, for n ≥ 3 the number of arrays x[0..n‑1] of
 *            values 0..5 such that x[i] ≥ (x[i‑1] + x[i‑2]) mod 6 for every
 *            i. We use a dynamic‑programming recurrence: f_k[a][b] = number
 *            of arrays of length k whose last two elements are a,b (a,b ∈
 *            {0,…,5}). Transition: for every a,b and every...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases first. */
    if (n == 0) return (seq_t)0;   /* undefined at n = 0, but harmless */
    if (n == 1) return (seq_t)6;   /* all 6 choices for the single element */

    /* Arrays that hold the counts for the current and next length. */
    seq_t cur[6][6];
    seq_t nxt[6][6];

    /* Initialise f_2: for length 2 an array is legal iff its second element
       is not smaller than the first (this follows from the wrap‑around
       condition). Thus f_2[a][b] = 1 if b ≥ a, otherwise 0. */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            cur[a][b] = (b >= a) ? (seq_t)1 : (seq_t)0;

    /* Sum over all pairs for length 2. */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            total += cur[a][b];

    /* For n = 2 we already have the answer. */
    if (n == 2) return total;

    /* Extend the DP for lengths 3 through n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero the next‑state array. */
        for (int a = 0; a < 6; ++a)
            for (int b = 0; b < 6; ++b)
                nxt[a][b] = (seq_t)0;

        /* Transition from cur to nxt. */
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                seq_t val = cur[a][b];
                if (val == (seq_t)0) continue;
                int s = (a + b) % 6;            /* lower bound for the next value */
                for (int c = s; c < 6; ++c) {
                    nxt[b][c] += val;
                }
            }
        }

        /* Compute the total for this length and prepare for the next step. */
        total = (seq_t)0;
        for (int a = 0; a < 6; ++a)
            for (int b = 0; b < 6; ++b)
                total += nxt[a][b];

        /* Copy nxt into cur for the next iteration. */
        for (int a = 0; a < 6; ++a)
            for (int b = 0; b < 6; ++b)
                cur[a][b] = nxt[a][b];
    }

    return total;
}
