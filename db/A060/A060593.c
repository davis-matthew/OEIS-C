/*
 * A060593  a(n) is the number of ways that a cycle of length 2n+1 in the
 *          symmetric group S_(2n+1) can be decomposed as the product of two
 *          cycles of length 2n+1.
 * https://oeis.org/A060593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n)! / (n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t limit = n * 2;        /* compute factorial of 2n */
    for (idx_t i = 2; i <= limit; ++i) {
        r *= (seq_t)i;
    }
    r /= ((seq_t)n + 1);
    return r;
}
