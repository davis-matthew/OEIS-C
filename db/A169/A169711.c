/*
 * A169711  The function W_n(6) (see Borwein et al. reference for definition).
 * https://oeis.org/A169711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^3 - 9*n^2 + 4*n
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 895, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once to avoid repeated conversions */
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;           /* n^2 */
    seq_t n3 = n2 * n_t;            /* n^3 */
    seq_t res = (seq_t)6 * n3        /* 6*n^3 */
              - (seq_t)9 * n2        /* 9*n^2 */
              + (seq_t)4 * n_t;      /* 4*n */
    return res;
}
