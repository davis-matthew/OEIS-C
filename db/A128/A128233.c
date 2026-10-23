/*
 * A128233  Average of p(n) and p(p(n)), where p(k) is the k-th prime.
 * https://oeis.org/A128233
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n) + p(p(n)))/2
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* behaviour below offset is irrelevant */

    /* p(n) */
    seq_t pn = (seq_t)get_prime(n);

    /* p(p(n)) – cast back to idx_t because get_prime expects an index */
    seq_t ppn = (seq_t)get_prime((idx_t)pn);

    /* average of the two primes */
    return (pn + ppn) / (seq_t)2;
}
