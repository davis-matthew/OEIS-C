/*
 * A215354  Primes congruent to {0, 2, 3, 4, 5} mod 11.
 * https://oeis.org/A215354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate primes and keep those with residue 0,2,3,4,5 modulo 11. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for incremental prime generation */
    static idx_t cur_candidate = 2;          /* next number to test */
    static uint64_t *all_primes   = NULL;    /* list of all primes found   */
    static size_t   all_len   = 0, all_cap = 0;
    static uint64_t *qual_primes = NULL;    /* primes satisfying the mod condition */
    static size_t   qual_len   = 0, qual_cap = 0;

    /* Keep generating until we have at least n qualifying primes */
    while (qual_len < (size_t)n) {
        uint64_t cand = (uint64_t)cur_candidate;
        int is_prime = 1;   /* assume prime until proven otherwise */

        /* Test divisibility by known primes (trial division) */
        if (cand == 2) {
            /* 2 is prime – no test needed */
        } else {
            for (size_t i = 0; i < all_len; ++i) {
                uint64_t p = all_primes[i];
                if (p > cand / p) break;          /* avoid overflow: p*p > cand */
                if (cand % p == 0) {               /* composite */
                    is_prime = 0;
                    break;
                }
            }
        }

        if (is_prime) {
            /* Store prime in the full list */
            if (all_len == all_cap) {
                all_cap = (all_cap == 0) ? 16 : all_cap * 2;
                all_primes = (uint64_t *)realloc(all_primes, all_cap * sizeof(uint64_t));
            }
            all_primes[all_len++] = cand;

            /* Check residue modulo 11 */
            uint64_t r = cand % 11;
            if (r == 0 || r == 2 || r == 3 || r == 4 || r == 5) {
                if (qual_len == qual_cap) {
                    qual_cap = (qual_cap == 0) ? 16 : qual_cap * 2;
                    qual_primes = (uint64_t *)realloc(qual_primes, qual_cap * sizeof(uint64_t));
                }
                qual_primes[qual_len++] = cand;
            }
        }

        /* Move to the next candidate: after 2 we skip even numbers */
        if (cand == 2)
            cur_candidate = 3;
        else
            cur_candidate += 2;
    }

    /* Return the nth qualifying prime (1‑based index) */
    return (seq_t)qual_primes[n - 1];
}
