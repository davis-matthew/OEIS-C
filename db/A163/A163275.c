/*
 * A163275  a(n) = n^5*(n+1)^2/2.
 * https://oeis.org/A163275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^5*(n+1)^2/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 623, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;          /* n in 128‑bit */
    unsigned __int128 np1 = N + 1;                      /* n+1 in 128‑bit */
    unsigned __int128 term = N * N * N * N * N;          /* n^5 */
    term = term * np1 * np1;                            /* * (n+1)^2 */
    term = term >> 1;                                   /* /2  (n^5*(n+1)^2 is even) */
    return (seq_t)term;
}
