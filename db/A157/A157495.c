/*
 * A157495  The smallest prime difference between prime(n) and any smaller
 *          square.
 * https://oeis.org/A157495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 8236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A157495: the smallest prime difference between the n‑th prime p
   and a smaller perfect square k² (k² < p).  Scan k from floor(sqrt(p))
   down to 1 and return the first p – k² that is prime. */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for indices below the offset */

    /* n‑th prime number */
    uint32_t p = get_prime(n);

    /* largest k with k² <= p  (quick integer sqrt) */
    idx_t k = 0;
    while ((k + 1) * (k + 1) <= (idx_t)p)
        ++k;

    /* search downward for the smallest prime difference */
    for (; k > 0; --k)
    {
        seq_t diff = (seq_t)p - (seq_t)k * (seq_t)k;
        if (diff >= (seq_t)2 &&
            is_prime_small((idx_t)diff))
            return diff;
    }

    /* no non‑zero square yielded a prime difference → return p itself */
    return (seq_t)p;
}
