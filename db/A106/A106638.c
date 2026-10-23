/*
 * A106638  Limiting word of the morphism 1->1221, 2->2332, 3->3113 starting
 *          from 1.
 * https://oeis.org/A106638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterated morphism: 1->1221, 2->2332, 3->3113 starting from 1 */
seq_t a(idx_t n) {
    /* mapping table: rows correspond to the current symbol (0 unused) */
    static const seq_t map[4][4] = {
        {0,0,0,0},
        {1,2,2,1},
        {2,3,3,2},
        {3,1,1,3}
    };

    /* Extract the base‑4 digits of n (least significant first). */
    idx_t temp = n;
    int digits[64];
    int len = 0;
    do {
        digits[len++] = temp % 4;
        temp /= 4;
    } while (temp != 0);

    /* Iterate over the digits from most significant to least,
       applying the morphism each time. */
    int sym = 1;                      /* start symbol 1 */
    for (int i = len - 1; i >= 0; --i) {
        sym = (int)map[sym][digits[i]];
    }

    return (seq_t)sym;
}
