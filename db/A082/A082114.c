/*
 * A082114  Diagonal sums of number array A082110.
 * https://oeis.org/A082114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(n^4 - n^3 + 26*n^2 - 26*n + 30)/30
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow on large n */
    unsigned __int128 N   = n;            /* copy index as 128‑bit value   */
    unsigned __int128 N2  = N * N;        /* n^2                            */
    unsigned __int128 N3  = N2 * N;       /* n^3                            */
    unsigned __int128 N4  = N3 * N;       /* n^4                            */
    unsigned __int128 tmp = N4 - N3 +       /* n^4 - n^3                      */
                           26 * N2 -            /* + 26 n^2                       */
                           26 * N +            /* - 26 n                         */
                           30;                 /* + 30                           */
    unsigned __int128 prod = (N + 1) * tmp;/* (n+1) * (expression)           */
    seq_t res = (seq_t)(prod / 30);        /* exact division by 30           */
    return res;
}
