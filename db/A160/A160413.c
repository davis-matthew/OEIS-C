/*
 * A160413  a(n) = A160411(n+1)/4.
 * https://oeis.org/A160413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; if n even: a(n) = n/2; if n odd ≥ 3: a(n) = 3*n – 2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;
    }
    if ((n & 1U) == 0U) {
        /* even n */
        return (seq_t)(n >> 1);
    }
    /* odd n ≥ 3 */
    seq_t t = (seq_t)n;
    return t * (seq_t)3 - (seq_t)2;
}
