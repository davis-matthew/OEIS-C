/*
 * A141086  a(n) = prime(2*n^2) - 2*n^2.
 * https://oeis.org/A141086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n^2) - 2*n^2
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; the formula is valid for n ≥ 1 */
    if (n == 0)
        return 0;               /* undefined for n < 1, harmless */

    /* Compute the index of the prime to fetch: k = 2*n^2 */
    idx_t k = 2 * n * n;

    /* Fetch the k‑th prime and subtract the index */
    seq_t p = (seq_t)get_prime(k);
    return p - (seq_t)k;
}
