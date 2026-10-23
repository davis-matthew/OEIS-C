/*
 * A118816  A fractal sequence based upon powers of 3.
 * https://oeis.org/A118816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 * a(n) = a(n/3) if n % 3 == 0 * a(n) = ⌊n/3⌋
 *            otherwise * This implements OEIS A118816 (fractal sequence based
 *            upon powers of 3)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Iterate while n is a multiple of 3, reducing it */
    while (n % 3 == 0) {
        n /= 3;                /* divide by 3 */
        if (n == 1) {          /* hit the base case during reduction */
            return (seq_t)1;
        }
    }

    /* At this point n % 3 != 0, so a(n) = floor(n/3) */
    return (seq_t)(n / 3);
}
