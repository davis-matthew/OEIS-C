/*
 * A228949  Coins left when packing boomerangs into n X n coins.
 * https://oeis.org/A228949
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n mod 3 = 0 a(n) = 2*n - 1 if n mod 3 = 1 a(n) = 3*n
 *            - 2 if n mod 3 = 2 (This expression reproduces OEIS A228949.)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // For indices below 2 the behavior is unspecified.
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;
    idx_t r = n % 3;

    if (r == 0)
        return nn;
    else if (r == 1)
        return 2 * nn - 1;
    else   /* r == 2 */
        return 3 * nn - 2;
}
