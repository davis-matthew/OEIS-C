/*
 * A100201  Primes of the form 23*k+3.
 * https://oeis.org/A100201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 3 (mod 23)
 * Range:     u8 n <= 2, u16 n <= 301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;  /* array of found primes */
    static idx_t   size   = 0;     /* number of stored primes   */
    static idx_t   cap    = 0;     /* allocated capacity       */
    static uint64_t next_k = 0;    /* next k to test for 23*k+3 */

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        uint64_t candidate = 23 * next_k + 3;   /* candidate prime */
        if (is_prime(candidate))
            push_seq(&primes, &cap, &size, (seq_t)candidate);
        ++next_k;
    }

    /* Return the (n‑1)th element (the nth prime in 1‑based indexing). */
    return primes[n - 1];
}
