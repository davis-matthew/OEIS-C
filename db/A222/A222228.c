/*
 * A222228  In the number n, replace all (decimal) digits '2' with '4' and
 *          vice versa.
 * https://oeis.org/A222228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace decimal digits 2 and 4 in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t factor = 1;
    while (n > 0) {
        unsigned digit = n % 10;
        unsigned mapped = (digit == 2) ? 4
                           : (digit == 4) ? 2
                           : digit;
        result += (seq_t)mapped * factor;
        factor *= 10;
        n /= 10;
    }
    return result;
}
