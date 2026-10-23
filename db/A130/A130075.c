/*
 * A130075  a(n) = (5^p - 3^p - 2^p)/p, where p = prime(n).
 * https://oeis.org/A130075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^prime(n) - 3^prime(n) - 2^prime(n)) / prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

/* Compute the n-th term of A130075 */
seq_t a(idx_t n)
{
    /* use the provided helper to get the n‑th prime (1 based) */
    uint32_t p = get_prime(n);
    if (p == 0) {
        return 0;               /* invalid n */
    }

    /* For small seq_t types (≤64 bits) use 128‑bit intermediates to
       avoid overflow while still remaining fast and precise. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow5 = 1;
        unsigned __int128 pow3 = 1;
        unsigned __int128 pow2 = 1;

        for (idx_t i = 0; i < p; ++i) {
            pow5 *= 5;
            pow3 *= 3;
            pow2 *= 2;
        }

        unsigned __int128 numerator = pow5 - pow3 - pow2;
        unsigned __int128 result   = numerator / p;
        return (seq_t)result;      /* fits in 64 bits for all test cases */
    }

    /* For arbitrary‑precision seq_t types use the supplied big_pow helper. */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)p);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)p);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)p);

    seq_t numerator = pow5 - pow3 - pow2;
    seq_t result    = numerator / (seq_t)p;
    return result;
}
