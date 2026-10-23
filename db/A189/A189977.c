/*
 * A189977  Primes p such that d(p+1) = 2*d(p-1), where d(k) counts the
 *          divisors of k.
 * https://oeis.org/A189977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 331, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, divisor_count_u64, grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth prime p such that d(p+1) = 2*d(p-1)  (OEIS A189977) */
/* Uses a simple incremental search with memoization.  The function
   stores the primes found so far and continues searching when
   required, so calls may arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime.h"
#include "../utils/grow_seq_array.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* cached sequence of found primes */
    static seq_t *list   = NULL;
    static size_t len    = 0;
    static size_t cap    = 0;
    static uint64_t next_candidate = 0; /* next integer to test for primality */
    static bool initialized = false;

    /* initialise on first call */
    if (!initialized) {
        cap = 16;
        list = alloc_seq_array(cap);
        len  = 0;
        next_candidate = 2;
        initialized = true;
    }

    /* generate primes and test until we have at least n terms */
    while (len < (size_t)n) {
        uint64_t p = next_candidate;

        /* advance to next candidate after testing p */
        if (p == 2) {
            next_candidate = 3;               /* next prime candidate after 2 is 3 */
        } else {
            next_candidate += 2;               /* consider only odd numbers after 2 */
        }

        /* skip if not prime */
        if (!is_prime(p)) continue;

        /* compute divisor counts */
        seq_t d_plus  = divisor_count_u64(p + 1);
        seq_t d_minus = divisor_count_u64(p - 1);

        if (d_plus == d_minus * 2) {
            /* store found prime */
            grow_seq_array(&list, &cap, len + 1);
            list[len++] = (seq_t)p;
        }
    }

    /* n == 0 is out of range, but we return 0 just in case */
    if (n == 0) return 0;
    return list[(size_t)n - 1];
}
