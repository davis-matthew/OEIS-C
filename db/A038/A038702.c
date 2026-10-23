/*
 * A038702  a(n) = prime(n)^2 mod prime(n-1).
 * https://oeis.org/A038702
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 mod prime(n-1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6771, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. */
    if (n < 2) {
        return 0;
    }

    /* prime(n) and prime(n-1) – indices are 1‑based as per the helper. */
    uint32_t p = get_prime(n);       /* prime(n) */
    uint32_t q = get_prime(n - 1);   /* prime(n-1) */

    /* Compute p² modulo q using seq_t arithmetic. */
    seq_t prod = (seq_t)p * (seq_t)p;
    return prod % (seq_t)q;
}
