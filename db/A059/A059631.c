/*
 * A059631  Carryless multiplication 8 X n base 10.
 * https://oeis.org/A059631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless multiplication of 8 by n in base 10, i.e. for
 *            each decimal digit d of n, replace it by (8*d mod 10) and
 *            assemble the digits into a decimal number with no carries
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res   = 0;
    seq_t mult  = 1;
    while (n > 0) {
        idx_t d   = n % 10;
        seq_t prod = ((seq_t)d * 8) % 10;
        res += prod * mult;
        mult *= (seq_t)10;
        n /= 10;
    }
    return res;
}
