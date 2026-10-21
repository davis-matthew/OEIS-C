/*
 * A067870  Numbers k such that k and 3^k end with the same digit.
 * https://oeis.org/A067870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A067870
 * a(n) = 10*n - 7   for n even
 * a(n) = 10*n - 3   for n odd
 * (n >= 1)
 */
seq_t a(idx_t n)
{
    /* compute 10*n */
    seq_t res = (seq_t)n * 10;

    /* adjust according to parity of n */
    if (n & 1)            /* odd n */
        res -= 3;
    else                  /* even n */
        res -= 7;

    return res;
}
