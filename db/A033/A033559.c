/*
 * A033559  a(n) = (q - p)/2, where p is the largest prime < n and q is the
 *          smallest prime > n.
 * https://oeis.org/A033559
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (q - p)/2 where p is the largest prime < n and q is the
 *            smallest prime > n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool in is_prime */
#include "../utils/is_prime.h"/* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* The function is defined for n >= 4. For smaller n behavior is
       unspecified; we return 0. */
    if (n < 4)
        return (seq_t)0;

    /* Find the largest prime p < n */
    idx_t p = n - 1;
    while (p >= 2 && !is_prime(p))
        --p;

    /* Find the smallest prime q > n */
    idx_t q = n + 1;
    while (!is_prime(q))
        ++q;

    /* Compute (q - p)/2 as a seq_t value */
    seq_t diff = (seq_t)(q - p);
    return diff / 2;
}
