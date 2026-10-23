/*
 * A164487  Number of binary strings of length n with no substrings equal to
 *          0001, 1010, or 1100.
 * https://oeis.org/A164487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 55, u64 n <= 113,
 *            u128 n <= 229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164487: number of binary strings of length n
 * with no substrings equal to 0001, 1010, or 1100.
 * Computed by dynamic programming over the last 3 bits
 * (8 states).  a(0)=1, a(1)=2, a(2)=4, a(3)=8. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    seq_t cur[8], nxt[8];
    int s, b, new_s, val4;

    /* initial state for strings of length 3: one occurrence of each 3‑bit pattern */
    for (s = 0; s < 8; ++s) cur[s] = (seq_t)1;

    for (idx_t i = 3; i < n; ++i) {
        for (s = 0; s < 8; ++s) nxt[s] = 0;

        for (s = 0; s < 8; ++s) {
            if (cur[s] == 0) continue;
            for (b = 0; b < 2; ++b) {
                val4 = (s << 1) | b;                /* 4‑bit window formed by last 3 bits + new bit */
                if (val4 == 1 || val4 == 10 || val4 == 12) /* forbidden patterns 0001,1010,1100 */
                    continue;
                new_s = val4 & 7;                   /* last 3 bits of the new string */
                nxt[new_s] += cur[s];
            }
        }

        for (s = 0; s < 8; ++s) cur[s] = nxt[s];
    }

    seq_t total = 0;
    for (s = 0; s < 8; ++s) total += cur[s];
    return total;
}
