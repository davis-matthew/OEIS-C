/*
 * A221512  Number of 0..5 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sequences of length n over {0..5} starting with
 *            0 such that each element differs from at least one neighbour by
 *            at least 2. Recurrence: a(1)=0, a(2)=4, a(3)=13, a(4)=80,
 *            a(5)=381, a(6)=1970, ... DP[prev][sat], where prev∈[0..5] and
 *            sat∈{0,1} indicates whether the element at the c...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ull)            /* offset is 1, but guard anyway */
        return (seq_t)0;
    if (n == 1ull)            /* only one element, cannot satisfy */
        return (seq_t)0;

    /* DP state: dp[prev][sat] */
    seq_t dp[6][2] = {{0}};
    seq_t ndp[6][2] = {{0}};

    /* initial state: first element is 0, no left neighbour */
    dp[0][0] = (seq_t)1;

    for (idx_t step = 1ull; step < n; ++step) {
        /* zero ndp */
        for (int p = 0; p < 6; ++p)
            for (int f = 0; f < 2; ++f)
                ndp[p][f] = (seq_t)0;

        for (int prev = 0; prev < 6; ++prev) {
            for (int sat = 0; sat < 2; ++sat) {
                seq_t ways = dp[prev][sat];
                if (!ways)
                    continue;
                for (int next = 0; next < 6; ++next) {
                    int diff = (prev > next) ? (prev - next) : (next - prev);
                    if (sat == 0 && diff < 2)
                        continue;               /* requires larger difference */
                    int newSat = (diff >= 2) ? 1 : 0;
                    ndp[next][newSat] += ways;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int p = 0; p < 6; ++p)
            for (int f = 0; f < 2; ++f)
                dp[p][f] = ndp[p][f];
    }

    /* last element must satisfy with its left neighbour */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 6; ++prev)
        result += dp[prev][1];

    return result;
}
