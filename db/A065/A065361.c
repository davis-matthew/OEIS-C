/*
 * A065361  Rebase n from 3 to 2. Replace 3^k with 2^k in ternary expansion of
 *          n.
 * https://oeis.org/A065361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{k≥0} (digit_k of n in base 3) * 2^k
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    unsigned int pos = 0;

    while (n > 0)
    {
        idx_t digit = n % 3;
        res += ((seq_t)digit) << pos;
        n /= 3;
        ++pos;
    }

    return res;
}
