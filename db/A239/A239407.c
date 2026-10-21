/*
 * A239407  Number of 3Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it, modulo 4.
 * https://oeis.org/A239407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of 3×n arrays over {0,1,2,3} satisfying:
 *            for each cell (i,j) its value is different from (1 + sum of
 *            preceding cells in the same row) mod 4 and from (2 + sum of
 *            cells above it in the same column) mod 4. The number is computed
 *            by dynamic programming over the cumulative row‑s...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   // below offset – value irrelevant
    const int STATES = 64;           // 4^3 possible residue triples
    seq_t curr[STATES];
    seq_t next[STATES];
    for (int i = 0; i < STATES; ++i)
        curr[i] = (seq_t)0;
    curr[0] = (seq_t)1;              // all row sums initially 0

    for (idx_t step = 0; step < n; ++step)
    {
        for (int i = 0; i < STATES; ++i)
            next[i] = (seq_t)0;

        for (int s = 0; s < STATES; ++s)
        {
            if (curr[s] == (seq_t)0)
                continue;

            int s0 = s / 16;               // highest two bits
            int rem = s % 16;
            int s1 = rem / 4;
            int s2 = rem % 4;

            int forb0_1 = (1 + s0) & 3;
            int forb0_2 = 2;                // constant

            for (int v0 = 0; v0 < 4; ++v0)
            {
                if (v0 == forb0_1 || v0 == forb0_2)
                    continue;

                int forb1_1 = (1 + s1) & 3;
                int forb1_2 = (2 + v0) & 3;

                for (int v1 = 0; v1 < 4; ++v1)
                {
                    if (v1 == forb1_1 || v1 == forb1_2)
                        continue;

                    int forb2_1 = (1 + s2) & 3;
                    int sum_above = (v0 + v1) & 3;     // (v0+v1) mod 4
                    int forb2_2 = (2 + sum_above) & 3;

                    for (int v2 = 0; v2 < 4; ++v2)
                    {
                        if (v2 == forb2_1 || v2 == forb2_2)
                            continue;

                        int ns0 = (s0 + v0) & 3;
                        int ns1 = (s1 + v1) & 3;
                        int ns2 = (s2 + v2) & 3;
                        int ns_id = ns0 * 16 + ns1 * 4 + ns2;

                        next[ns_id] = next[ns_id] + curr[s];
                    }
                }
            }
        }

        for (int i = 0; i < STATES; ++i)
            curr[i] = next[i];
    }

    seq_t ans = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        ans = ans + curr[i];
    return ans;
}
