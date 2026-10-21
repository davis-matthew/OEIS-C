/*
 * A214079  a(n) = ceiling( n^(1/3) )!.
 * https://oeis.org/A214079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(n^(1/3))!
 * Range:     u8 n <= 63, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

/* Compute k = ceil(cube_root(n)) */
static idx_t ceil_cube_root(idx_t n)
{
    /* floor cube root */
    seq_t f = floor_cube_root((seq_t)n);

    /* convert f to 64‑bit integer for the next steps */
    idx_t fk = (idx_t)f;

    /* compute f^3 in 128‑bit to avoid overflow */
    unsigned __int128 f3 = (unsigned __int128)fk * fk * fk;
    unsigned __int128 n128 = (unsigned __int128)n;

    if (f3 == n128) return fk;          /* exact cube root */
    return fk + 1;                      /* otherwise ceil */
}

/* Simple factorial computation */
static seq_t factorial_of(idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= k; ++i)
        res *= (seq_t)i;
    return res;
}

/* Target function for OEIS A214079 */
seq_t a(idx_t n)
{
    idx_t k = ceil_cube_root(n);
    return factorial_of(k);
}
