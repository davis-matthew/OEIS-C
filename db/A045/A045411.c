/*
 * A045411  Primes congruent to {1, 3, 5, 6} mod 7.
 * https://oeis.org/A045411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045411: primes congruent to {1,3,5,6} mod 7.
   The function generates primes by trial division, stores all primes
   found so far, and keeps a filtered list that contains only the
   primes that satisfy the residue condition.  The algorithm is
   incremental and works for arbitrary‑order calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Increase the capacity of a dynamic uint64_t array.  */
static void ensure_capacity(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < need) new_cap = need;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* Return the n-th prime congruent to 1,3,5, or 6 modulo 7. */
seq_t a(idx_t n)
{
    /* All primes found so far. */
    static uint64_t *all_primes   = NULL;
    static size_t  all_cap       = 0;
    static idx_t   all_cnt       = 0;

    /* Primes that satisfy the residue condition. */
    static uint64_t *filtered     = NULL;
    static size_t   filt_cap       = 0;
    static idx_t    filt_cnt       = 0;

    /* Next odd number to test. */
    static idx_t    next_candidate = 3;

    /* Generate primes until we have at least n filtered primes. */
    while (filt_cnt < n) {
        idx_t cand = next_candidate;
        next_candidate = cand + 2;        /* skip even numbers */

        /* Trial division using previously found primes. */
        bool is_prime = true;
        for (idx_t i = 0; i < all_cnt; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime for future tests. */
            ensure_capacity(&all_primes, &all_cap, all_cnt + 1);
            all_primes[all_cnt++] = (uint64_t)cand;

            /* Check the residue condition; if it matches, store it. */
            idx_t r = cand % 7;
            if (r != 0 && r != 2 && r != 4) {
                ensure_capacity(&filtered, &filt_cap, filt_cnt + 1);
                filtered[filt_cnt++] = (uint64_t)cand;
            }
        }
    }

    return (seq_t)filtered[n - 1];
}
