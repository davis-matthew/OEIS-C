/*
 * A093019  a(n) = A093018(n) mod 10.
 * https://oeis.org/A093019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Luhn check digit of n (decimal representation)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t pos = 0;
    idx_t x       = n;
    while (x) {
        seq_t d = x % 10;
        if ((pos & 1) == 0) {        /* double every second digit from the right */
            d <<= 1;
            if (d > 9) d -= 9;       /* sum digits of the doubled value */
        }
        sum += d;
        pos++;
        x /= 10;
    }
    seq_t m = sum % 10;
    return (m == 0) ? 0 : (10 - m);
}
