/*
 * A099278  Unsigned member r=-20 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) + 21*a(n-2) - a(n-3) for n >= 3, a(0) = 0, a(1)
 *            = 1, a(2) = 20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)20;

    seq_t a_n_minus3 = (seq_t)0;      /* a(0) */
    seq_t a_n_minus2 = (seq_t)1;      /* a(1) */
    seq_t a_n_minus1 = (seq_t)20;     /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)21 * a_n_minus1;          /* 21*a(n-1) */
        cur = cur + (seq_t)21 * a_n_minus2;    /* +21*a(n-2) */
        cur = cur - a_n_minus3;                /* -a(n-3) */
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = cur;
    }

    return a_n_minus1;
}
