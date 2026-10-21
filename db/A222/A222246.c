/*
 * A222246  In the number n, replace all (decimal) digits '5' with '7' and
 *          vice versa.
 * https://oeis.org/A222246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) swaps digits 5 and 7 in the decimal representation of n
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n;
    int len = 0;
    do {
        ++len;
        temp /= 10;
    } while (temp > 0);
    seq_t res = 0;
    idx_t pow = 1;
    for (int i = 1; i < len; ++i)
        pow *= 10;
    for (int i = 0; i < len; ++i) {
        idx_t digit = (n / pow) % 10;
        if (digit == 5)
            digit = 7;
        else if (digit == 7)
            digit = 5;
        res = res * 10 + (seq_t)digit;
        pow /= 10;
    }
    return res;
}
