/*
 * A115353  The mode of the digits of n (using smallest mode if multimodal).
 * https://oeis.org/A115353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest digit with maximal frequency in decimal
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned char cnt[10] = {0};
    idx_t x = n;
    if (x == 0UL) {
        cnt[0] = 1;
    } else {
        while (x > 0) {
            unsigned d = x % 10;
            cnt[d] += 1;
            x /= 10;
        }
    }
    unsigned char maxcnt = 0;
    unsigned char mode = 0;
    for (unsigned d = 0; d < 10; ++d) {
        if (cnt[d] > maxcnt) {
            maxcnt = cnt[d];
            mode = d;
        } else if (cnt[d] == maxcnt && d < mode) {
            mode = d;
        }
    }
    return (seq_t)mode;
}
