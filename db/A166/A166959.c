/*
 * A166959  Numbers congruent to (12,32) mod 44.
 * https://oeis.org/A166959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 22*n - (-1)^n - 11
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the equivalent piecewise form: */
/*   if n is even: a(n) = 22*n - 12 */
/*   if n is odd:  a(n) = 22*n - 10 */

seq_t a(idx_t n)
{
    /* Compute 22*n first */
    seq_t res = (seq_t)22 * (seq_t)n;

    /* Adjust according to parity of n */
    if ((n & 1ULL) == 0)          /* even n */
        res -= 12;
    else                          /* odd n */
        res -= 10;

    return res;
}
