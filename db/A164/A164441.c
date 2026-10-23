/*
 * A164441  Number of binary strings of length n with no substrings equal to
 *          0000, 0110, or 1111.
 * https://oeis.org/A164441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164441: number of binary strings of length n with no substrings
   equal to 0000, 0110, or 1111.
   Computed by a linear‑time DP over the automaton of the last 3 bits. */
seq_t a(idx_t n)
{
    /* Small n handled directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* cur holds counts for strings of current length ending with a 3‑bit suffix */
    seq_t cur[8];
    for (int s = 0; s < 8; ++s) cur[s] = (seq_t)1;   /* length 3 */

    seq_t next[8];
    for (idx_t len = 4; len <= n; ++len) {
        for (int i = 0; i < 8; ++i) next[i] = 0;

        for (int s = 0; s < 8; ++s) {
            seq_t val = cur[s];
            if (!val) continue;

            /* try appending bit 0 */
            unsigned int four = ((s << 1) | 0u) & 0xFu;
            if (four != 0u && four != 6u && four != 15u) {
                int to = ((s << 1) | 0u) & 7;
                next[to] += val;
            }

            /* try appending bit 1 */
            four = ((s << 1) | 1u) & 0xFu;
            if (four != 0u && four != 6u && four != 15u) {
                int to = ((s << 1) | 1u) & 7;
                next[to] += val;
            }
        }

        for (int i = 0; i < 8; ++i) cur[i] = next[i];
    }

    /* sum all suffix counts to get the total */
    seq_t total = 0;
    for (int i = 0; i < 8; ++i) total += cur[i];
    return total;
}
