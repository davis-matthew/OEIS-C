/*
 * A040982  Primes p such that x^5 = 19 has a solution mod p.
 * https://oeis.org/A040982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p for which x^5 ≡ 19 (mod p) has a solution. Tested via
 *            the criterion: 19^((p-1)/gcd(5,p-1)) ≡ 1 (mod p). Since 19 ≡ 0
 *            (mod 19), the prime 19 must be accepted separately
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *prime_cache = NULL;     /* primes satisfying the condition */
static size_t cache_len   = 0;         /* number of primes stored */
static size_t cache_cap   = 0;         /* capacity of prime_cache */
static idx_t   next_candidate = 2;     /* next integer to test for primality */

/* grow the static cache if necessary */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);          /* fatal allocation failure */
        prime_cache = tmp;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but avoid UB */

    /* extend cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {
            bool ok = false;

            /* special‑case p = 19 (19 | 19) */
            if (next_candidate == 19) {
                ok = true;
            } else {
                uint64_t g = ((next_candidate - 1) % 5ULL == 0ULL) ? 5ULL : 1ULL;
                uint64_t exp = (next_candidate - 1) / g;
                seq_t mod  = (seq_t)next_candidate;
                seq_t res  = mod_pow((seq_t)19, exp, mod);
                ok = (res == (seq_t)1);
            }

            if (ok) {
                grow_cache();
                prime_cache[cache_len++] = (seq_t)next_candidate;
            }
        }

        /* advance to the next integer to test */
        if (next_candidate < 3)
            next_candidate = 3;   /* skip 2 -> 3 */
        else
            next_candidate += 2;  /* skip even numbers afterwards */
    }

    return prime_cache[n - 1];
}
