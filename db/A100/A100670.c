/*
 * A100670  Number of two-card Baccarat hands of point n.
 * https://oeis.org/A100670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of unordered two‑card hands whose point is n (mod
 *            10). Each card type is counted once per rank and suit: value 0
 *            has 16 types, each value 1–9 has 4 types. Cards may be identical
 *            (from different decks). The result is the count of unordered
 *            pairs (i, j) with i + j ≡ n (mod 10) where i...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = n % 10;
    /* number of distinct card types for each point value */
    static const idx_t valcnt[10] = {
        16, /* 0-valued cards: 10, J, Q, K (4 each) */
        4, 4, 4, 4, 4, 4, 4, 4, 4 /* 1–9-valued cards: A,2–9 */
    };

    seq_t res = 0;
    for (idx_t i = 0; i < 10; ++i) {
        for (idx_t j = i; j < 10; ++j) {
            if ( (i + j) % 10 == mod ) {
                if (i == j) {
                    seq_t c = (seq_t)valcnt[i];
                    res += c * (c + (seq_t)1) / (seq_t)2;   /* C(c+1,2) */
                } else {
                    res += (seq_t)valcnt[i] * (seq_t)valcnt[j];
                }
            }
        }
    }
    return res;
}
