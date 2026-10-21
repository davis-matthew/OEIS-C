/*
 * A065166  Permutation t->t+3 of Z, folded to N.
 * https://oeis.org/A065166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(g(n)+3) where f(z) = 2z if z > 0 2|z| + 1 otherwise
 *            g(z) = z/2 if z is even (1-z)/2 if z is odd
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1) == 0) {
        seq_t k = (seq_t)(n / 2);
        return 2 * k + 6;
    }

    /* odd indices */
    seq_t k = (seq_t)((n - 1) / 2);
    if (k <= 2)       /* m > 0 case: 6 - 2k   */
        return 6 - 2 * k;
    if (k == 3)       /* m = 0 case: 1       */
        return (seq_t)1;
    return 2 * k - 5; /* m < 0 case: 2k - 5   */
}
