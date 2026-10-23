/*
 * A138636  a(n) = 6 * prime(n).
 * https://oeis.org/A138636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * prime(n)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* For n=0 the OEIS offset does not matter, return 0 */
    if (n == 0)
        return 0;
    /* Retrieve the n-th prime (1‑based) and multiply by 6 */
    seq_t p = (seq_t)get_prime(n);
    return p * 6;
}
