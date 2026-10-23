/*
 * A191258  Fixed point of the morphism 0->01, 1->02, 2->03, 3->001.
 * https://oeis.org/A191258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th symbol (1‑based) of the fixed point of the
 *            morphism 0 → 01, 1 → 02, 2 → 03, 3 → 001
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* out of range protection */
        return (seq_t)0;

    /* length tables for each depth: len[d][s] = length of the word
       obtained by expanding symbol s after d derivations */
    idx_t len[64][4];
    int depth = 0;
    len[0][0] = len[0][1] = len[0][2] = len[0][3] = 1;

    /* grow until the total length of the word starting from 0 reaches n */
    while (len[depth][0] < n) {
        depth++;
        len[depth][0] = len[depth - 1][0] + len[depth - 1][1];
        len[depth][1] = len[depth - 1][0] + len[depth - 1][2];
        len[depth][2] = len[depth - 1][0] + len[depth - 1][3];
        len[depth][3] = len[depth - 1][0] + len[depth - 1][0]
                        + len[depth - 1][1];
    }

    int symbol = 0;            /* start with the seed symbol 0 */
    idx_t pos = n - 1;         /* zero‑based position inside the word */

    /* descend from the computed depth to the original symbol */
    for (int d = depth; d > 0; --d) {
        const idx_t *prev = len[d - 1];
        switch (symbol) {
            case 0: {
                idx_t l0 = prev[0];
                if (pos < l0)
                    symbol = 0;
                else {
                    pos -= l0;
                    symbol = 1;
                }
                break;
            }
            case 1: {
                idx_t l0 = prev[0];
                if (pos < l0)
                    symbol = 0;
                else {
                    pos -= l0;
                    symbol = 2;
                }
                break;
            }
            case 2: {
                idx_t l0 = prev[0];
                if (pos < l0)
                    symbol = 0;
                else {
                    pos -= l0;
                    symbol = 3;
                }
                break;
            }
            case 3: {
                idx_t l0 = prev[0];
                if (pos < l0) {
                    symbol = 0;
                } else if (pos < l0 + l0) {
                    pos -= l0;
                    symbol = 0;
                } else {
                    pos -= l0 + l0;
                    symbol = 1;
                }
                break;
            }
        }
    }

    return (seq_t)symbol;
}
