/*
 * A239813  Number of nX2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A239813
   Count of n × 2 arrays of values 0..3 such that for each cell
   the value is not equal to (sum of elements to its left) mod 4
   nor to (sum of elements above it + 1) mod 4.
   This is computed by a simple O(n) dynamic program over the
   current column sums modulo 4. */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* DP arrays for current and next row.
       cur[s1][s2] = #ways after having processed some rows,
       where s1,s2 are the cumulative sums modulo 4 for columns 1 and 2. */
    seq_t cur[4][4] = {0};
    seq_t nxt[4][4] = {0};

    cur[0][0] = 1;          /* no rows processed yet */

    for (idx_t row = 0; row < n; ++row) {
        /* clear next-row array */
        for (int s1 = 0; s1 < 4; ++s1)
            for (int s2 = 0; s2 < 4; ++s2)
                nxt[s1][s2] = 0;

        for (int s1 = 0; s1 < 4; ++s1) {
            for (int s2 = 0; s2 < 4; ++s2) {
                seq_t ways = cur[s1][s2];
                if (!ways) continue;

                /* allowed values for a1 in current row */
                int forbidden1 = (s1 + 1) & 3;     /* (sum_above + 1) mod 4 */
                int a1_list[3];
                int n1;

                if (s1 == 3) {                  /* forbidden only 0 */
                    a1_list[0] = 1;
                    a1_list[1] = 2;
                    a1_list[2] = 3;
                    n1 = 3;
                } else {                         /* forbidden 0 and forbidden1 */
                    n1 = 0;
                    for (int a1 = 1; a1 <= 3; ++a1) {
                        if (a1 == forbidden1) continue;
                        a1_list[n1++] = a1;
                    }
                }

                for (int k1 = 0; k1 < n1; ++k1) {
                    int a1 = a1_list[k1];
                    int s1new = (s1 + a1) & 3;

                    int forbidden2 = (s2 + 1) & 3;     /* (sum_above + 1) mod 4 for column 2 */
                    int a2_list[3];
                    int n2 = 0;

                    for (int a2 = 0; a2 <= 3; ++a2) {
                        if (a2 == a1) continue;          /* cannot equal a1 */
                        if (a2 == forbidden2) continue;   /* cannot equal forbidden2 */
                        a2_list[n2++] = a2;
                    }

                    for (int k2 = 0; k2 < n2; ++k2) {
                        int a2 = a2_list[k2];
                        int s2new = (s2 + a2) & 3;
                        nxt[s1new][s2new] += ways;
                    }
                }
            }
        }

        /* move next to current for the next iteration */
        for (int s1 = 0; s1 < 4; ++s1)
            for (int s2 = 0; s2 < 4; ++s2)
                cur[s1][s2] = nxt[s1][s2];
    }

    /* sum all possibilities for the last row */
    seq_t total = 0;
    for (int s1 = 0; s1 < 4; ++s1)
        for (int s2 = 0; s2 < 4; ++s2)
            total += cur[s1][s2];
    return total;
}
