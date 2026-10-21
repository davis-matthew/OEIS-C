/*
 * A066406  a(n) = 2^n*(3^n-3).
 * https://oeis.org/A066406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(3^n-3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(1) = 0 */

seq_t a(idx_t n)
{
    /* Handle the offset; A066406 is defined for n >= 1 */
    if (n == 0) {
        return 0;            /* not used but safe */
    }

    /* Compute 2^n and 3^n iteratively (O(n) time) */
    seq_t pow2 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
    }

    /* a(n) = 2^n * (3^n - 3) */
    seq_t diff = pow3 - (seq_t)3;
    return pow2 * diff;
}
