/*
 * A020334  Numbers whose base-6 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*6^floor(log_6(n)+1) + n
 * Range:     u8 n <= 6, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case (not used in the OEIS offset) */
    if (n == 0) return (seq_t)0;

    /* Find the number of base‑6 digits of n (len = floor(log6(n)) + 1) */
    idx_t t = n;
    unsigned len = 1;
    while (t >= 6) {
        t /= 6;
        ++len;
    }

    /* Compute 6^len as seq_t */
    seq_t pow6 = (seq_t)1;
    for (unsigned i = 0; i < len; ++i) {
        pow6 *= (seq_t)6;
    }

    /* Return n * pow6 + n */
    seq_t nn = (seq_t)n;
    return nn * pow6 + nn;
}
