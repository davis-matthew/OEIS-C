/*
 * A078688  Continued fraction expansion of e^(1/4).
 * https://oeis.org/A078688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = 1, unless n % 3 == 1, then a(n) = 8 * ((n-1)/3) + 3
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 1) {
        seq_t k = (seq_t)(n - 1) / 3;  /* (n-1)/3 as seq_t */
        return 8 * k + 3;
    }
    return 1;
}
