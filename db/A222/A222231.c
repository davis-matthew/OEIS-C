/*
 * A222231  In the number n, replace all (decimal) digits '2' with '7' and
 *          vice versa.
 * https://oeis.org/A222231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by swapping decimal digits 2 and 7 in n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 0;
    seq_t pow10 = 1;
    while (nn != 0) {
        seq_t digit = nn % 10;
        if (digit == 2) digit = 7;
        else if (digit == 7) digit = 2;
        res += digit * pow10;
        pow10 *= 10;
        nn /= 10;
    }
    return res;
}
