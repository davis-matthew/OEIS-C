/*
 * A222236  In the number n, replace all (decimal) digits '3' with '6' and
 *          vice versa.
 * https://oeis.org/A222236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace every decimal digit 3 with 6 and 6 with 3 in n
 * Range:     u8 n <= 229, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t factor = 1;
    idx_t temp = n;
    while (temp > 0)
    {
        unsigned d = temp % 10;
        unsigned swapped = (d == 3) ? 6 : ((d == 6) ? 3 : d);
        res = res + factor * swapped;
        factor = factor * 10;
        temp = temp / 10;
    }
    return res;
}
