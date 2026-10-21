/*
 * A207914  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A207914 (number of n×4 binary arrays avoiding 000,101 horizontally and 010,101 vertically).
   The algorithm uses a transfer‑matrix DP on pairs of consecutive rows.
   Initial values: a(1)=9, a(2)=81.  Subsequent values are obtained by iteratively
   applying the transition table for pairs of valid rows. */

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0) return (seq_t)0;          /* value not defined for n=0, but defined for completeness */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    /* valid 4‑bit rows that satisfy the horizontal constraints:
       0010,0011,0100,0110,0111,1001,1100,1110,1111 (decimal 2,3,4,6,7,9,12,14,15) */
    static const int patterns[9] = {2, 3, 4, 6, 7, 9, 12, 14, 15};

    /* pre‑compute all allowed transitions between pairs of rows.
       State id = prevprev_index * R + prev_index (R = 9). */
    static unsigned char allowed[81][9];   /* 81 states × 9 possible next rows */
    static int initialized = 0;
    if (!initialized) {
        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                int id = a * 9 + b;
                for (int k = 0; k < 9; ++k) {
                    int c = k;
                    int ok = 1;
                    for (int col = 0; col < 4; ++col) {
                        int bit_a = (patterns[a] >> (3 - col)) & 1;
                        int bit_b = (patterns[b] >> (3 - col)) & 1;
                        int bit_c = (patterns[c] >> (3 - col)) & 1;
                        if ((bit_a == 0 && bit_b == 1 && bit_c == 0) ||
                            (bit_a == 1 && bit_b == 0 && bit_c == 1)) {
                            ok = 0;
                            break;
                        }
                    }
                    allowed[id][k] = (unsigned char)ok;
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays holding counts for each state */
    seq_t cur[81], next[81];
    for (int i = 0; i < 81; ++i) cur[i] = (seq_t)1;   /* all pairs of rows exist for n = 2 */

    /* iterate rows from 3 to n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 81; ++i) next[i] = (seq_t)0;

        for (int id = 0; id < 81; ++id) {
            if (!cur[id]) continue;
            int i_prevprev = id / 9;
            int i_prev     = id % 9;

            for (int k = 0; k < 9; ++k) {
                if (allowed[id][k]) {
                    int next_id = i_prev * 9 + k;
                    next[next_id] += cur[id];
                }
            }
        }

        /* swap arrays for next iteration */
        for (int i = 0; i < 81; ++i) cur[i] = next[i];
    }

    /* sum over all states to get the total number of arrays of height n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 81; ++i) total += cur[i];

    return total;
}
