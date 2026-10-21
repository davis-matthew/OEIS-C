/*
 * A055003  a(n) = prime(prime(n)-1).
 * https://oeis.org/A055003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)-1)
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* get_prime(k) returns the k‑th prime.  We rely on the provided helper. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset: the first term corresponds to n = 1 */
    if (n < 1)
        return (seq_t)0; /* undefined below the offset */

    /* p = prime(n) */
    seq_t p = (seq_t)get_prime(n);

    /* idx = p – 1 is the 1‑based index of the desired prime */
    idx_t idx = (idx_t)p - 1;

    /* a(n) = prime(idx) */
    return (seq_t)get_prime(idx);
}
