/*
 * A008954  Final digit of triangular number n*(n+1)/2.
 * https://oeis.org/A008954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n and n+1 modulo 20 to avoid large intermediate values.
       Since n*(n+1) is always even, (n*(n+1) mod 20)/2 gives the correct
       value of the triangular number modulo 10. */
    idx_t nmod  = n % 20ull;
    idx_t n1mod = (n + 1u) % 20ull;
    idx_t prod  = nmod * n1mod;        /* safe in idx_t (64‑bit) */
    idx_t prod20 = prod % 20ull;       /* 0 <= prod20 < 20 */
    seq_t res = (seq_t)(prod20 / 2u) % 10u;
    return res;
}
