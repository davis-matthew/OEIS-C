/*
 * A164319  Primes p such that the sum of divisors of p+1 is larger than 2*p.
 * https://oeis.org/A164319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1508, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   grow_seq_array, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164319: primes p such that σ(p+1) > 2p (search by primes) */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/grow_seq_array.h"

/* static storage for memoisation */
static seq_t *stored_primes = NULL;      /* array of found primes      */
static size_t stored_len   = 0;          /* number of primes stored   */
static size_t stored_cap   = 0;          /* allocated capacity       */
static idx_t  next_candidate = 2;        /* next integer to test     */

/* Return the n‑th term (1‑based) of A164319 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defensive, undefined */

    while (stored_len < n) {
        idx_t p = 0;

        /* find the next prime */
        for (;;) {
            if (next_candidate <= 2) {
                p = 2;
                next_candidate = 3;
            } else {
                /* skip even numbers */
                if ((next_candidate & 1) == 0) {
                    ++next_candidate;
                    continue;
                }
                p = next_candidate;
                ++next_candidate;
            }
            if (is_prime(p)) break;       /* p is prime */
        }

        /* compute σ(p+1) and compare with 2p */
        seq_t sigma = sigma_of(p + 1);
        seq_t twoP   = (seq_t)p;
        twoP       *= 2;

        if (sigma > twoP) {
            /* grow storage if needed */
            if (stored_len == stored_cap) {
                size_t new_cap = stored_cap ? stored_cap * 2 : 16;
                grow_seq_array(&stored_primes, &stored_cap, new_cap);
            }
            stored_primes[stored_len++] = (seq_t)p;
        }
    }

    return stored_primes[n - 1];
}
