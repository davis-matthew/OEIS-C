/*
 * A159237  Composite numbers A046346 divided by the sum of their prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A159237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046346(n) / Sopfr(A046346(n))
 * Range:     u8 n <= 163, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A046346 is the sequence of composite numbers (the n-th composite).
   Sopfr(m) is the sum of the prime factors of m counted with multiplicity
   (OEIS A001414).  For composite m, we list the ratio m / Sopfr(m)
   whenever m is divisible by its sum of prime factors.  The n‑th such
   ratio (with n ≥ 1) is a(n).  The implementation caches previously
   found values so that calls may arrive in any order.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"

/* Return the n-th term of sequence A159237, with n ≥ 1.  */
seq_t a(idx_t n)
{
    if (n == 0)                         /* not used, only for safety */
        return (seq_t)0;

    /* ------- static cache for memoisation ------- */
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_cap  = 0;      /* allocated capacity       */
    static size_t terms_found = 0;     /* number of terms stored   */
    static idx_t  last_checked = 1;    /* last composite examined */

    /* ---- enlarge cache if we need to store more terms ---- */
    if (cache_cap < n) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp)               /* allocation failure – give up */
            return (seq_t)0;
        if (cache) {
            memcpy(tmp, cache, terms_found * sizeof(seq_t));
            free(cache);
        }
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* ---- Return immediately if we already have the term ---- */
    if (terms_found >= n)
        return cache[n - 1];

    /* ---- Find additional terms until we reach n ---- */
    idx_t current = last_checked;          /* start after the last checked */
    while (terms_found < n) {
        ++current;

        /* Skip primes: A046346 consists only of composites */
        if (is_prime(current))
            continue;

        /* Compute Sopfr(current) by repeated division by smallest prime factor */
        uint64_t temp      = (uint64_t)current;
        uint64_t sum_pf    = 0;
        while (temp > 1) {
            uint64_t p = _smallest_prime_factor(temp);
            sum_pf += p;
            temp /= p;
        }
        if (sum_pf == 0)       /* should not happen for composites */
            continue;

        /* Check divisibility condition */
        if (current % sum_pf == 0) {
            cache[terms_found] = (seq_t)(current / sum_pf);
            ++terms_found;
        }
    }

    /* Remember where we stopped for future calls */
    last_checked = current;

    return cache[n - 1];
}
