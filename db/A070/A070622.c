/*
 * A070622  a(n) = n^5 mod 39.
 * https://oeis.org/A070622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 39
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n modulo 39 first – the sequence is periodic with period 39 */
    idx_t r = n % 39u;
    seq_t m = (seq_t)r;

    /* compute m^5 modulo 39 with 64‑bit intermediate to avoid overflow
       (for 8‑bit seq_t this is essential). */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
    {
        uint64_t prod = ((uint64_t)res * (uint64_t)m) % 39;
        res = (seq_t)prod;
    }

    return res;
}
