/*
 * A083445  Largest n-digit number minus the product of its digits; i.e., a(n)
 *          = 99999... (n 9's) - 9^n.
 * https://oeis.org/A083445
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (10^n - 1) - 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow10 = 1;
    seq_t pow9  = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= 10;
        pow9  *= 9;
    }
    return pow10 - 1 - pow9;
}
