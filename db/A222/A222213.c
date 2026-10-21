/*
 * A222213  Replace all (decimal) digits '0' with '4' and vice versa.
 * https://oeis.org/A222213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace all decimal digits 0 with 4 and all digits 4 with
 *            0 in n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;  // special case: "0" becomes "4"

    seq_t result = 0;
    seq_t mult = 1;
    idx_t temp = n;

    while (temp > 0) {
        unsigned d = temp % 10;     // single decimal digit
        if (d == 0) d = 4;
        else if (d == 4) d = 0;

        result += (seq_t)d * mult;
        mult *= (seq_t)10;
        temp /= 10;
    }
    return result;
}
