/*
 * A175109  a(n) = ((2*n+1)^3+(-1)^n)/2.
 * https://oeis.org/A175109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n+1)^3 + (-1)^n) / 2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 813, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote index to sequence type */
    seq_t m = (seq_t)n;

    /* Compute (2*n + 1) */
    seq_t two_n_plus_one = m * 2 + 1;

    /* Cube it */
    seq_t cube = two_n_plus_one * two_n_plus_one * two_n_plus_one;

    /* Adjust by (-1)^n: add 1 if n even, subtract 1 if n odd */
    if (n & 1)
        cube -= 1;
    else
        cube += 1;

    /* Divide by 2 – the result is guaranteed to be even */
    return cube / 2;
}
