/*
 * A000981  Numbers beginning with letter 'n' in English.
 * https://oeis.org/A000981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   the English name starts with 'n' iff the leading 3-digit group g
 *            is 9, 19, 90..99 or 900..999. So for each k >= 1 there are
 *            112*1000^(k-1) terms of the form g*1000^(k-1) + r with 0 <= r <
 *            1000^(k-1), in increasing order
 * Range:     u8 n <= 12, u16 n <= 2112, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t M = 1;
    /* find the block (number of 3-digit groups) containing index n */
    while (1) {
        if (M <= n / 112 && n > (idx_t)112 * M) {
            n -= (idx_t)112 * M;
            M *= 1000;
        } else {
            break;
        }
    }
    idx_t idx = n - 1;
    idx_t li = idx / M;
    idx_t rem = idx % M;
    idx_t g;
    if (li == 0) g = 9;
    else if (li == 1) g = 19;
    else if (li <= 11) g = 88 + li;
    else g = 888 + li;
    return (seq_t)g * (seq_t)M + (seq_t)rem;
}
