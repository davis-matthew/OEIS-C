/*
 * A191269  Fixed point of the morphism 0 -> 001, 1 -> 02, 2 -> 01.
 * https://oeis.org/A191269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth symbol of the fixed point of the morphism 0 → 001, 1
 *            → 02, 2 → 01 (starting symbol 0), offset 1. The algorithm
 *            computes the lengths of the letter expansions up to the depth
 *            covering n, then walks down the expansion tree selecting the
 *            child that contains the desired position
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* maximum depth needed for n <= 10000 is far less than 64,
       but we reserve 64 to be safe for larger n. */
    idx_t l0[64], l1[64], l2[64];
    idx_t depth = 0;

    /* base depth 0: a single letter */
    l0[0] = l1[0] = l2[0] = (idx_t)1;

    /* grow lengths until the expansion of 0 covers position n */
    while (l0[depth] < n) {
        ++depth;
        l0[depth] = 2 * l0[depth - 1] + l1[depth - 1];
        l1[depth] = l0[depth - 1] + l2[depth - 1];
        l2[depth] = l0[depth - 1] + l1[depth - 1];
    }

    idx_t pos = n;          /* current 1‑based position in the
                               expansion of the current letter */
    int letter = 0;         /* start from the root symbol 0 */

    /* descend from the computed depth to the bottom */
    for (idx_t d = depth; d > 0; --d) {
        idx_t len0 = l0[d - 1];
        idx_t len1 = l1[d - 1];
        idx_t len2 = l2[d - 1];

        if (letter == 0) {
            /* expansion "001" : 0, 0, 1 */
            if (pos <= len0) {
                /* first 0, no change to pos */
                letter = 0;
            } else if (pos <= len0 + len0) {
                /* second 0 */
                pos -= len0;
                letter = 0;
            } else {
                /* the 1 */
                pos -= 2 * len0;
                letter = 1;
            }
        } else if (letter == 1) {
            /* expansion "02" : 0, 2 */
            if (pos <= len0) {
                letter = 0;
            } else {
                pos -= len0;
                letter = 2;
            }
        } else { /* letter == 2 */
            /* expansion "01" : 0, 1 */
            if (pos <= len0) {
                letter = 0;
            } else {
                pos -= len0;
                letter = 1;
            }
        }
    }

    return (seq_t)letter;
}
