/*
 * A175224  a(n) = prime(n) + 8.
 * https://oeis.org/A175224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 8
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Returns the n-th prime (1‑based) and adds 8.  The function works for
   any idx_t and seq_t supplied by the build system.  It does not
   store any state, so calls can arrive in any order.  For n up to
   ~10 000 the search is well under 2 s. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    idx_t count     = 0;
    idx_t candidate = 2;

    /* Scan candidates until we have found the n-th prime. */
    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }

    return (seq_t)candidate + (seq_t)8;
}
