/*
 * A172225  Number of ways to place 2 nonattacking wazirs on an n X n board.
 * https://oeis.org/A172225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2+n-4)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n < 1 the sequence is not defined in OEIS, but we
       return 0 for safety. */
    if (n < 1)
        return 0;

    seq_t N      = (seq_t)n;
    seq_t term1  = N * (N - 1);
    seq_t inner  = N * N + N;   /* n^2 + n */
    inner = inner - (seq_t)4;   /* + n^2 + n - 4 */

    seq_t val = term1 * inner;
    val = val / (seq_t)2;
    return val;
}
