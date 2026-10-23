/*
 * A141978  Primes congruent to 2 mod 29.
 * https://oeis.org/A141978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 29 == 2 (primes congruent
 *            to 2 mod 29)
 * Range:     u8 n <= 3, u16 n <= 237, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for computed terms and auxiliary primes */
static seq_t *mod2_primes = NULL;        /* primes ≡ 2 (mod 29) */
static size_t mod2_len = 0, mod2_cap = 0;
static seq_t *all_primes  = NULL;        /* all primes found so far */
static size_t all_len = 0, all_cap = 0;
static seq_t next_candidate = 3;          /* next odd to test */
static bool initialized = false;

/* ensure we have at least n terms in mod2_primes */
static void
ensure_terms(idx_t n)
{
    if (!initialized) {
        /* initialise with prime 2 */
        all_cap = 16;
        mod2_cap = 16;
        all_primes = (seq_t *)malloc(all_cap * sizeof(seq_t));
        mod2_primes = (seq_t *)malloc(mod2_cap * sizeof(seq_t));
        all_primes[0] = (seq_t)2;
        mod2_primes[0] = (seq_t)2;     /* 2 % 29 == 2 */
        all_len = mod2_len = 1;
        next_candidate = 3;
        initialized = true;
    }

    while (mod2_len < n) {
        seq_t cand = next_candidate;

        /* find the next prime */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < all_len; ++i) {
                seq_t p = all_primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* store to all_primes */
                if (all_len == all_cap) {
                    all_cap = (all_cap ? all_cap * 2 : 16);
                    all_primes = (seq_t *)realloc(all_primes, all_cap * sizeof(seq_t));
                }
                all_primes[all_len++] = cand;

                /* store if congruent to 2 mod 29 */
                if ((cand % (seq_t)29) == 2) {
                    if (mod2_len == mod2_cap) {
                        mod2_cap = (mod2_cap ? mod2_cap * 2 : 16);
                        mod2_primes = (seq_t *)realloc(mod2_primes, mod2_cap * sizeof(seq_t));
                    }
                    mod2_primes[mod2_len++] = cand;
                }

                next_candidate = cand + 2;
                break;          /* prime found, go to next outer iteration */
            }
            cand += 2;           /* test next odd number */
        }
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but defensive */
    ensure_terms(n);
    return mod2_primes[(size_t)n - 1];
}
