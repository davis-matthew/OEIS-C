/*
 * A207939  Number of 3 X n 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6, a(2)=36.
   For n≥3 the value is obtained by a dynamic‑program over columns.
   A 3×n binary array is counted iff
     • each column is not 001 (1) or 110 (6) vertically,
     • in every row no horizontal window 001 or 100 occurs.
   The DP keeps the last two columns (3 bits each → 64 states)
   and for each state tries all allowed next columns, checking the
   horizontal constraints.  Complexity: O(n·64·6). */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* not used, keep defined */
    if (n == 1u) return (seq_t)6;   /* a(1) = 6 */
    if (n == 2u) return (seq_t)36;  /* a(2) = 36 */

    /* Allowed columns: all 3‑bit columns except 001 (1) and 110 (6). */
    static const idx_t allowed[6] = {0, 2, 3, 4, 5, 7};

    /* DP arrays: 64 states = 2 columns * 3 bits each. */
    seq_t dp_cur[64] = {0}, dp_next[64] = {0};

    /* Initialise for the first two columns. */
    for (int i = 0; i < 6; ++i) {
        int c1 = (int)allowed[i];
        for (int j = 0; j < 6; ++j) {
            int c2 = (int)allowed[j];
            int state = 0;
            for (int r = 0; r < 3; ++r) {
                int b1 = (c1 >> r) & 1;
                int b2 = (c2 >> r) & 1;
                state |= (b1 << (r * 2)) | (b2 << (r * 2 + 1));
            }
            dp_cur[state] += (seq_t)1;
        }
    }

    /* Process columns 3 … n. */
    for (idx_t col = 3u; col <= n; ++col) {
        /* Zero dp_next.  Use explicit assignment, not memset, so that
           it works for non‑trivial seq_t types. */
        for (int s = 0; s < 64; ++s) dp_next[s] = (seq_t)0;

        for (int st = 0; st < 64; ++st) {
            seq_t cur = dp_cur[st];
            if (!cur) continue;

            /* Extract the last two columns' bits for each row. */
            int last1[3], last2[3];
            for (int r = 0; r < 3; ++r) {
                last1[r] = (st >> (r * 2)) & 1;        /* column i-2 */
                last2[r] = (st >> (r * 2 + 1)) & 1;    /* column i-1 */
            }

            for (int k = 0; k < 6; ++k) {
                int colBits = (int)allowed[k];
                int ok = 1;
                for (int r = 0; r < 3; ++r) {
                    int nb = (colBits >> r) & 1;
                    /* horizontal window patterns 001 or 100 are forbidden. */
                    if ((last1[r] == 0 && last2[r] == 0 && nb == 1) ||
                        (last1[r] == 1 && last2[r] == 0 && nb == 0)) {
                        ok = 0; break;
                    }
                }
                if (!ok) continue;

                /* Build new state: shift the last two columns. */
                int newSt = 0;
                for (int r = 0; r < 3; ++r) {
                    int nb = (colBits >> r) & 1;
                    newSt |= (last2[r] << (r * 2)) | (nb << (r * 2 + 1));
                }
                dp_next[newSt] += cur;
            }
        }

        /* Copy dp_next to dp_cur.  Use explicit assignment. */
        for (int s = 0; s < 64; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all remaining states to get the final count. */
    seq_t sum = (seq_t)0;
    for (int s = 0; s < 64; ++s) sum += dp_cur[s];
    return sum;
}
