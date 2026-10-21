/*
 * A227203  Prime(n)^2 mod (prime(n) + prime(n+1)).
 * https://oeis.org/A227203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(n)^2 mod (Prime(n)+Prime(n+1))
 * Range:     u8 n <= 30, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* a(n) is defined for n >= 1. Return 0 for n == 0 (offset handling) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;         /* how many primes seen so far   */
    idx_t candidate = 1;     /* candidate numbers start at 2 */
    uint64_t p = 0, q = 0;   /* will hold prime(n) and prime(n+1) */

    /* Find the n-th and (n+1)-th primes via simple trial‑division */
    while (count < n + 1) {
        ++candidate;
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                p = (uint64_t)candidate;
            if (count == n + 1)
                q = (uint64_t)candidate;
        }
    }

    seq_t mod = (seq_t)(p + q);               /* prime(n)+prime(n+1)   */
    seq_t val = ((seq_t)p * (seq_t)p) % mod;  /* prime(n)^2 mod mod    */
    return val;
}
