/*
 * A157618  a(n) = 625*n^2 - 886*n + 314.
 * https://oeis.org/A157618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 625*n^2 - 886*n + 314
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 2622, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;                     /* cast index to seq_t */
    seq_t res = 625;                 /* start with coefficient 625 */
    res *= N;                        /* 625*n           */
    res *= N;                        /* 625*n^2         */
    res -= 886 * N;                  /* minus 886*n     */
    res += 314;                      /* plus 314        */
    return res;
}
