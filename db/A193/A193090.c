/*
 * A193090  Digital roots of the nonzero pentagonal numbers.
 * https://oeis.org/A193090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root(n*(3*n-1)/2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by reducing the product n*(3*n-1) modulo 18, then dividing
   by 2 to obtain the pentagonal number modulo 9.  The digital
   root is that residue unless it is 0, in which case the root is 9. */
seq_t a(idx_t n)
{
    /* n % 18 */
    seq_t n_mod18 = (seq_t)n % 18;
    /* 3*n % 18 */
    seq_t three_n_mod18 = (seq_t)3 * n_mod18 % 18;
    /* (3*n - 1) % 18, handle wrap-around */
    seq_t m = (three_n_mod18 + 18 - 1) % 18;
    /* numerator modulo 18: n*(3*n-1) % 18 */
    seq_t t = n_mod18 * m % 18;
    /* divide by 2 (numerator is always even) */
    seq_t root = t / 2;           /* root in [0,8] */
    return (root == 0) ? (seq_t)9 : root;
}
