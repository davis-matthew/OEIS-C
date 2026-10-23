/*
 * A119994  Fixed point of the morphism a -> {a, a + 1, Prime(a + 1)},
 *          beginning with 0.
 * https://oeis.org/A119994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
 * For k ≥ 0:
 *   a(3k)   = a(k);
 *   a(3k+1) = a(k) + 1;
 *   a(3k+2) = Prime(a(k) + 1)   (Prime(x) is the x‑th prime)
 *
 * This implements the fixed point of the morphism
 * a → {a, a+1, Prime(a+1)} starting with 0. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t parent = n / 3;
    unsigned rem   = (unsigned)(n % 3);

    seq_t val = a(parent);

    if (rem == 0)
        return val;
    else if (rem == 1)
        return val + 1;
    else /* rem == 2 */
    {
        /* val + 1 is the index of the prime to take */
        idx_t k = (idx_t)(val + 1);
        /* get_prime returns the k-th prime as uint32_t */
        return (seq_t)get_prime(k);
    }
}
