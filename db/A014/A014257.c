/*
 * A014257  Product of digits of 2^n.
 * https://oeis.org/A014257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the decimal digits of 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= 2;
    }

    /* product of digits of pow */
    seq_t prod = 1;
    seq_t x    = pow;

    /* special case: when 2^n == 0 (impossible but kept for safety) */
    if (x == 0) {
        return 0;
    }

    while (x > 0) {
        seq_t d = x % 10;
        prod   *= d;
        x      /= 10;
    }

    return prod;
}
