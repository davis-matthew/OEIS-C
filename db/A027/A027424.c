/*
 * A027424  Number of distinct products i*j with 1 <= i, j <= n (number of
 *          distinct terms in n X n multiplication table).
 * https://oeis.org/A027424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of distinct products i*j with 1 ≤ i,j ≤ n
 * Range:     u8 n <= 27, u16 n <= 502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exact computation of the Nth term of OEIS A027424.
   The routine works for any seq_t type (built‑in or big integer)
   and any idx_t type (here assumed to be unsigned 64‑bit). */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Valid only for n ≥ 1, return 0 otherwise. */
    if (n < 1) return 0;

    /* Use size_t for array indices – large enough for our range. */
    size_t nn = (size_t)n;
    size_t maxprod = nn * nn;                 /* largest product i*j */
    size_t bytes   = (maxprod + 8) >> 3;       /* bits → bytes */

    /* Allocate a zeroed bitset: one bit per possible product. */
    uint8_t *bits = (uint8_t *)calloc(bytes, 1);
    if (!bits) return 0;                      /* allocation failure → 0 */

    /* Mark every product i*j. */
    for (size_t i = 1; i <= nn; ++i) {
        for (size_t j = 1; j <= nn; ++j) {
            size_t prod = i * j;
            bits[prod >> 3] |= (uint8_t)(1u << (prod & 7));
        }
    }

    /* Count set bits – that is the number of distinct products. */
    seq_t count = 0;
    for (size_t k = 0; k < bytes; ++k) {
        count += (seq_t)__builtin_popcount((unsigned int)bits[k]);
    }

    free(bits);
    return count;
}
