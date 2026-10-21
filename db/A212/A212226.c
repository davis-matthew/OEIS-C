/*
 * A212226  Number of 0..2 arrays of length n+3 with sum no more than 4 in any
 *          length 4 subsequence (=50% duty cycle).
 * https://oeis.org/A212226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 45, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(n) = 2*a(n-1)+a(n-3)+6*a(n-4)-3*a(n-5)-a(n-6)+a(n-7)
 *                -7*a(n-8)+4*a(n-9)-a(n-10)+3*a(n-12)-4*a(n-13)-a(n-16)+a(n-17)
 * Initial values (for n ≥ 1):
 *    a(1)=50, a(2)=124, a(3)=311, a(4)=775, a(5)=1895,
 *    a(6)=4663, a(7)=11518, a(8)=28446, a(9)=70145, a(10)=172951,
 *    a(11)=426630, a(12)=1052487, a(13)=2596184, a(14)=6403675,
 *    a(15)=15795627, a(16)=38963018, a(17)=96109484.
 *
 * This implementation does not use the empirical recurrence.  It
 * counts the number of sequences of length n+3 over the alphabet {0,1,2}
 * subject to the sliding‑window constraint that any four consecutive
 * entries sum to at most 4.  The algorithm is a linear‑time dynamic
 * program that keeps track of the last three digits of the sequence
 * (27 possible states).  For each step it adds a new digit that
 * satisfies the constraint, building the counts for length L = n+3.
 * The result is returned as a seq_t value.
 */

seq_t a(idx_t n)
{
    if (n < 1)  /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    /* Length of the array is n+3 */
    idx_t L = n + 3;

    /* DP arrays for the 27 possible 3‑tuples (x,y,z) with values 0,1,2.
     * curr[i] holds the number of sequences of the current length that
     * end with the 3‑tuple encoded by i.
     */
    seq_t curr[27];
    seq_t next[27];

    /* Initial state: length 3, each triple occurs exactly once. */
    for (int i = 0; i < 27; ++i)
        curr[i] = 1;

    /* Build up to length L.  For length 4 and beyond, we apply the
     * sliding‑window constraint: x + y + z + d <= 4.
     */
    for (idx_t len = 4; len <= L; ++len) {
        /* zero the next array */
        for (int i = 0; i < 27; ++i)
            next[i] = 0;

        for (int i = 0; i < 27; ++i) {
            seq_t cnt = curr[i];
            int x = i / 9;          /* most significant digit */
            int y = (i / 3) % 3;    /* middle digit */
            int z = i % 3;          /* least significant digit */

            for (int d = 0; d <= 2; ++d) {
                if (x + y + z + d <= 4) {
                    int j = y * 9 + z * 3 + d;  /* new 3‑tuple (y,z,d) */
                    next[j] += cnt;
                }
            }
        }

        /* swap buffers */
        for (int i = 0; i < 27; ++i)
            curr[i] = next[i];
    }

    /* Sum all counts to obtain the total number of valid sequences. */
    seq_t total = 0;
    for (int i = 0; i < 27; ++i)
        total += curr[i];

    return total;
}
