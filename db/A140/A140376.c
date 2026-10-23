/*
 * A140376  Nonprimes of the form 26n+1.
 * https://oeis.org/A140376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 26*k+1 where k is the nth non‑negative integer such that
 *            26*k+1 is composite
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    idx_t count = 0;
    idx_t k = 0;

    for (;;) {
        idx_t candidate = 26 * k + 1;   /* 26*k+1 is the candidate number */
        if (!is_prime(candidate)) {     /* skip primes, count composites */
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++k;
    }
}
