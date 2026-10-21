/*
 * A054801  Second term of balanced prime quartets: p(m)-p(m-1) = p(m+1)-p(m)
 *          = p(m+2)-p(m+1).
 * https://oeis.org/A054801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced prime quartets: return the second prime of consecutive
   quadruples of primes with equal gaps.  The algorithm sieves primes
   incrementally and records each quartet found.  Results are memoised
   so that calls in any order are answered in O(1) after the
   necessary amounts of preprocessing. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility headers providing the required helpers. */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

/* Static state for memoisation and prime generation. */
static seq_t *a_vals   = NULL;   /* stored a(n) values            */
static size_t a_size  = 0;       /* number of stored values       */
static size_t a_cap   = 0;       /* current capacity of a_vals    */

static uint64_t *primes  = NULL; /* array of primes up to sieve_limit */
static size_t    primes_cnt = 0; /* number of primes                 */
static idx_t     sieve_limit = 0;/* maximum prime value in primes[]  */
static idx_t     last_scan   = 0;/* next quadruple start index to test */

/* Extend the sieve up to at least new_limit, rebuilding the prime list. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    if (primes)
        free(primes);

    build_primes_upto(new_limit, &primes, &primes_cnt);
    sieve_limit = new_limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* safeguard, offset is 1 */

    /* Generate terms until we have at least n of them. */
    while (a_size < n) {
        /* Ensure we have enough primes left to form a quartet. */
        if (primes_cnt - last_scan < 4) {
            idx_t new_limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
            while (true) {
                extend_sieve(new_limit);
                if (primes_cnt - last_scan >= 4)
                    break;
                new_limit *= 2;
            }
        }

        /* Scan quadruples starting at last_scan. */
        for (; last_scan + 3 < primes_cnt && a_size < n; ++last_scan) {
            uint64_t d1 = primes[last_scan + 1] - primes[last_scan];
            uint64_t d2 = primes[last_scan + 2] - primes[last_scan + 1];
            uint64_t d3 = primes[last_scan + 3] - primes[last_scan + 2];
            if (d1 == d2 && d2 == d3) {
                /* Ensure capacity for new term. */
                if (a_size + 1 > a_cap) {
                    size_t new_cap = (a_cap == 0) ? 8 : a_cap * 2;
                    a_vals = seq_realloc(a_vals, a_cap, new_cap);
                    a_cap = new_cap;
                }
                a_vals[a_size++] = (seq_t)primes[last_scan + 1];
            }
        }
    }

    return a_vals[n - 1];
}
