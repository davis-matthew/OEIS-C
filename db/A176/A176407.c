/*
 * A176407  Omit all 1's and 2's from n.
 * https://oeis.org/A176407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal integer obtained by deleting all digits 1 and
 *            2 from n
 * Range:     u8 n <= 299, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mult   = 1;

    while (n > 0) {
        unsigned d = n % 10;
        if (d != 1 && d != 2) {
            result += (seq_t)d * mult;
            mult   *= 10;
        }
        n /= 10;
    }

    return result;
}
