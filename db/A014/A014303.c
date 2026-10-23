/*
 * A014303  a(n) = prime(n)*(prime(n+1)-1)/2.
 * https://oeis.org/A014303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*(prime(n+1)-1)/2
 * Range:     u8 n <= 8, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Obtain the n‑th and (n+1)-th primes. */
    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    /* Promote to seq_t for the final arithmetic. */
    seq_t sp = (seq_t)p;
    seq_t sq = (seq_t)q;

    seq_t minus1 = sq - (seq_t)1;

    /* Divide one of the factors by two first to avoid overflow
       on small (e.g. 8‑bit) seq_t types. */
    if ((sp & (seq_t)1) == 0) {
        sp = sp >> 1;                /* sp is even */
    } else {
        minus1 = minus1 >> 1;        /* (q‑1) is even */
    }

    return sp * minus1;              /* product already divided by 2 */
}
