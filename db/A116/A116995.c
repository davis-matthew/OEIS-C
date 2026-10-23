/*
 * A116995  Pentagonal numbers with prime indices.
 * https://oeis.org/A116995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * (3*prime(n)-1) / 2
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* not defined for non‑positive indices */

    uint32_t p = get_prime(n);                /* n‑th prime (32‑bit) */
    seq_t prime = (seq_t)p;                   /* promote to seq_t      */
    seq_t res   = prime * ((seq_t)3 * prime - 1) / 2;
    return res;
}
