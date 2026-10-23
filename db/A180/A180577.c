/*
 * A180577  The Wiener index of the windmill graph D(6,n). The windmill graph
 *          D(m,n) is the graph obtained by taking n copies of the complete
 *          graph K_m with a vertex in common (i.e., a bouquet of n pieces of
 *          K_m graphs).
 * https://oeis.org/A180577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(5*n-2)
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* cast index to seq_t */
    seq_t five_n     = 5 * n_t;    /* five_n = 5*n */
    seq_t five_n_minus2 = five_n - 2;  /* five_n_minus2 = 5*n-2 */
    return 5 * n_t * five_n_minus2;
}
