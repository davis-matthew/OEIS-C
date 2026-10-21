/*
 * A222218  In the number n, replace all (decimal) digits '0' with '9' and
 *          vice versa.
 * https://oeis.org/A222218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   replace decimal digit 0 with 9 and digit 9 with 0 in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned char buf[20];
    unsigned int sz = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        buf[sz++] = 9;
    } else {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            unsigned nd = (d == 0) ? 9 : (d == 9) ? 0 : d;
            buf[sz++] = nd;
            tmp /= 10;
        }
    }
    seq_t res = 0;
    for (unsigned int i = sz; i-- > 0; ) {
        res = res * 10 + (seq_t)buf[i];
    }
    return res;
}
