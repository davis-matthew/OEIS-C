/*
 * A229277  Number of ascending runs in {1,...,3}^n.
 * https://oeis.org/A229277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3^(n-1)*(2*n+1) for n>0, a(0) = 0
 * (Derived from OEIS A229277)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t pow = 1;           /* 3^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow *= 3;

    seq_t two_n_plus1 = (seq_t)2 * n + 1;
    return pow * two_n_plus1;
}
