/*
 * A118286  Numbers n such that n == 0 (mod 4) or n == 2 (mod 12).
 * https://oeis.org/A118286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 1 + cos(n*Pi/2)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Cosine values: n mod 4 = 0 → 1, 1 → 0, 2 → -1, 3 → 0.  */

/*  seq_t and idx_t are provided by the build.  */
seq_t a(idx_t n)
{
    /* 3*n - 1, all operations are in seq_t to avoid overflow of idx_t. */
    seq_t val = (seq_t)3 * n - 1;

    /* Determine cos(n*π/2) by the remainder of n modulo 4. */
    idx_t r = n & 3;          /* n % 4, fast because 4 is a power of 2 */

    if (r == 0)               /* cos = +1 */
        val += 1;
    else if (r == 2)          /* cos = -1 */
        val -= 1;
    /* r == 1 or 3 → cos = 0, no change */

    return val;
}
