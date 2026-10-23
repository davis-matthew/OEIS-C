/*
 * A035107  First differences give (essentially) A028242.
 * https://oeis.org/A035107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 +54*n^2 +212*n +153 -9*(-1)^n)/48
 * Range:     u8 n <= 0, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;

    seq_t numerator = 4 * n3 + 54 * n2 + 212 * sn;

    /* 153 - 9*(-1)^n:  add 9 if n is odd, subtract 9 if n is even */
    if (n & 1)
        numerator += (seq_t)(153 + 9);   /* n odd  → 162 */
    else
        numerator += (seq_t)(153 - 9);   /* n even → 144 */

    return numerator / (seq_t)48;
}
