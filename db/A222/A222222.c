/*
 * A222222  In the number n, replace all (decimal) digits '1' with '5' and
 *          vice versa.
 * https://oeis.org/A222222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   replace digit 1 with 5 and digit 5 with 1 in decimal
 *            representation of n
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow10 = 1;
    idx_t tmp = n;

    while (tmp > 0)
    {
        unsigned d = tmp % 10;
        tmp /= 10;
        unsigned r = (d == 1) ? 5 : (d == 5) ? 1 : d;
        res += (seq_t)r * pow10;
        pow10 *= (seq_t)10;
    }

    return res;
}
