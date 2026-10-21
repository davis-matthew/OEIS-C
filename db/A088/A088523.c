/*
 * A088523  a(1) = 2; for n > 1, a(n) = (a(n-1) + prime(n)) mod n.
 * https://oeis.org/A088523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; for n > 1, a(n) = (a(n-1) + prime(n)) mod n
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* The behavior below the offset is irrelevant */
    if (n == 0) return (seq_t)0;

    /* a(1) is defined to be 2 */
    if (n == 1) return (seq_t)2;

    seq_t prev = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* prime(i) is 1‑based (prime(1)=2) */
        seq_t p    = (seq_t)get_prime(i);     /* n‑th prime as seq_t */
        seq_t sum  = prev + p;
        seq_t modv = (seq_t)i;
        prev      = (sum % modv);
    }

    return prev;
}
