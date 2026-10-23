/*
 * A034584  Radon-Hurwitz numbers: log_2 of dimension of an irreducible
 *          R-module for Clifford algebra Cl_n.
 * https://oeis.org/A034584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+8) = a(n) + 4,  a(0)=0, a(1)=1, a(2)=a(3)=2,
   a(4)=a(5)=a(6)=a(7)=3 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {0, 1, 2, 2, 3, 3, 3, 3};
    seq_t cycles = (seq_t)(n / 8);      /* how many complete 8‑blocks */
    size_t rem   = (size_t)(n % 8);     /* index within the block   */
    return cycles * 4 + base[rem];
}
