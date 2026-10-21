/*
 * A073965  Number of strings of length n over Z_5 with trace 0 and subtrace
 *          2.
 * https://oeis.org/A073965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings of length n over Z_5 with trace 0 and
 *            subtrace 2 Computation via dynamic programming on the trace and
 *            subtrace states
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* dp[t][s] holds the count for the current string length */
    seq_t cur[5][5] = {0};

    /* Base: length 1, trace equals the element, subtrace = 0 (no pairs) */
    for (int t = 0; t < 5; ++t)
        cur[t][0] = 1;

    /* a(1) is handled immediately */
    if (n == 1)
        return cur[0][2];

    seq_t nxt[5][5];

    /* Iterate through lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        /* Zero the next array */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                nxt[t][s] = 0;

        /* Transition for each state (t,s) */
        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t sum = 0;
                for (int a = 0; a < 5; ++a) {
                    int old_t = (t + 5 - a) % 5;          /* previous trace */
                    int prod  = (a * old_t) % 5;          /* a * old_t modulo 5 */
                    int old_s = (s + 5 - prod) % 5;       /* previous subtrace */
                    sum += cur[old_t][old_s];
                }
                nxt[t][s] = sum;
            }
        }

        /* Prepare for the next iteration */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                cur[t][s] = nxt[t][s];
    }

    /* The required term is trace 0, subtrace 2 */
    return cur[0][2];
}
