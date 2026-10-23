/*
 * A171818  Lunar product 2 X n in base 10.
 * https://oeis.org/A171818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number obtained by replacing each decimal digit of n
 *            with min(d, 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t tmp = n;
    int digits[20];
    int dcnt = 0;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d > 2) d = 2;
        digits[dcnt++] = d;
        tmp /= 10;
    }

    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
