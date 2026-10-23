/*
 * A046359  Odd numbers divisible by the palindromic sum of their prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A046359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   factor_u64, is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th element of OEIS sequence A046359.
 *
 *  An element is an odd positive integer N such that
 *
 *      S = sum of the prime factors of N (counted with multiplicity)
 *
 *  is a palindromic decimal number and N is a multiple of S.
 *
 *  The algorithm walks through odd integers, skips primes, and checks
 *  the palindrome and divisibility conditions.  New terms are appended
 *  to a static cache so that calls in any order are handled efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /*--- Static cache of computed terms ---------------------------------*/
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t cache_len = 0;     /* # stored terms   */
    static size_t cache_cap = 0;     /* capacity of array */

    /*--- Next odd candidate to examine ---------------------------------*/
    static uint64_t last_checked = 1;   /* holds next odd to test */

    /*--- Return cached value if it exists ------------------------------*/
    if (n <= cache_len)
        return cache[n - 1];

    /*--- Temporary storage for factorization ---------------------------*/
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /*--- Search until we have found the n‑th term ----------------------*/
    for (uint64_t cand = last_checked; ; cand += 2) {
        /* Factor cand */
        factor_u64(cand, primes, exps, &cnt);

        /* Skip 1 and primes (cnt==1 && exps[0]==1) */
        if (cand == 1 || (cnt == 1 && exps[0] == 1))
            continue;

        /* Compute sum of prime factors, counted with multiplicity */
        seq_t sum = 0;
        for (size_t i = 0; i < cnt; ++i)
            sum += (seq_t)primes[i] * (seq_t)exps[i];

        if (sum == 0 || !is_palindrome(sum, 10))
            continue;

        /* Convert sum to uint64_t for the modulus test (safe
           because sum fits in uint64_t for candidates up to UINT64_MAX) */
        uint64_t sum_u64 = (uint64_t)sum;
        if (cand % sum_u64 != 0)
            continue;

        /* New valid term found: store it */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                exit(EXIT_FAILURE);     /* allocation failure */
            }
            cache     = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cand;

        /* If we have reached the requested index, return the value */
        if (cache_len >= n) {
            last_checked = cand + 2;   /* next odd to test */
            return cache[n - 1];
        }
    }
}
