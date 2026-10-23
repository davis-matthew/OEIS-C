/*
 * A192848  Molecular topological indices of the graph join C_n + C_n of cycle
 *          graphs.
 * https://oeis.org/A192848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+2)*(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;       /* Convert index to seq_t */
    seq_t term = n_val * (n_val + 2);   /* n*(n+2) */
    term *= (2 * n_val - 1);          /* (n*(n+2))*(2n-1) */
    term *= 4;                        /* 4*n*(n+2)*(2n-1) */
    return term;
}
