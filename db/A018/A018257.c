/*
 * A018257  Divisors of 40.
 * https://oeis.org/A018257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 40 in ascending order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t divs[8] = {1, 2, 4, 5, 8, 10, 20, 40};
    if (n >= 1 && n <= 8) return divs[n - 1];
    return 0;
}
