/*
 * A083726  a(n) = (prime(n)+1)*n.
 * https://oeis.org/A083726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(prime(n)+1)
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t p = get_prime(n);          /* prime(n) with 1‑based indexing */
    return (seq_t)n * (p + 1);
}
