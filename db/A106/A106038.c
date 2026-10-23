/*
 * A106038  Triangle read by rows, based on the morphism f: 1->1231, 2->2112,
 *          3->3113.
 * https://oeis.org/A106038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth symbol in the fixed point of the morphism 1 →
 *            1231, 2 → 2112, 3 → 3113 (starting from symbol 1), obtained by
 *            reading the expansion in base‑4 representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t symbol = 1;                             /* root symbol */
    /* children of each symbol: {1,2,3} → {1231, 2112, 3113} */
    static const seq_t child[4][4] = {
        {0,0,0,0},          /* unused 0 index */
        {1,2,3,1},          /* children of 1 */
        {2,1,1,2},          /* children of 2 */
        {3,1,1,3}           /* children of 3 */
    };

    /* collect base‑4 digits of n (least significant first) */
    uint64_t tmp = n;
    seq_t digits[32];       /* 4^32 > 2^64, so 32 digits suffice for idx_t */
    int d = 0;
    while (tmp) {
        digits[d++] = (seq_t)(tmp & 3);  /* tmp % 4 */
        tmp >>= 2;                       /* tmp / 4 */
    }

    /* traverse digits from most significant to least to obtain the symbol */
    for (int i = d - 1; i >= 0; --i) {
        symbol = child[(int)symbol][(int)digits[i]];
    }
    return symbol;
}
