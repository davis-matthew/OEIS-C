/*
 * A225805  Sort the digits of n into alphabetical order according their
 *          French name: The "French Obsessive Filer's Sequence".
 * https://oeis.org/A225805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A225805: sort digits of n according to French names order */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned char cnt[10] = {0};
    idx_t t = n;
    while (t > 0) {
        unsigned d = t % 10;
        cnt[d]++;
        t /= 10;
    }

    static const unsigned char order[10] = {5, 2, 8, 9, 4, 7, 6, 3, 1, 0};

    seq_t res = 0;
    for (int i = 0; i < 10; ++i) {
        unsigned d = order[i];
        unsigned c = cnt[d];
        while (c--) {
            res = res * (seq_t)10 + (seq_t)d;
        }
    }
    return res;
}
