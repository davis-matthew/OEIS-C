/*
 * A073790  Numbers in base -9.
 * https://oeis.org/A073790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the decimal integer formed by the digits of n written in
 *            base -9 (digits 0–8)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    int digits[128];
    int len = 0;
    __int128 m = n;          /* use signed 128 bits to avoid overflow */
    const int base = -9;

    while (m != 0) {
        int r = (int)(m % base);   /* r might be negative */
        if (r < 0) r += 9;
        digits[len++] = r;
        m = (m - r) / base;
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
