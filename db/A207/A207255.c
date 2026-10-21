/*
 * A207255  Number of 4 X n 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5/6)*n^4 + (35/3)*n^3 - (5/6)*n^2 - (5/3)*n Equivalent
 *            integer form: a(n) = (5*n^4 + 70*n^3 - 5*n^2 - 10*n) / 6 for n ≥
 *            1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on 32‑bit seq_t builds. */
    __int128 N = (unsigned long long)n;          /* N = n */
    __int128 num = 5 * N * N * N * N             /* 5*n^4 */
               + 70 * N * N * N                  /* 70*n^3 */
               - 5 * N * N                       /* -5*n^2 */
               - 10 * N;                         /* -10*n */
    num /= 6;                                   /* divide by 6, exact division */
    return (seq_t)num;                          /* cast back to seq_t */
}
