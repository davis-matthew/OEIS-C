/*
 * A227921  Odd odious numbers (A000069), all divisors of which are odious.
 * https://oeis.org/A227921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 5763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, odious_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Output: OEIS A227921
   Formula: a(n) = first odd number whose every divisor is odious
   (i.e. binary representation contains an odd number of 1‑bits).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/odious_uint64.h"

/* Check that every divisor of x is odious.  x is assumed odd and < 2^64. */
static bool all_divisors_odious(uint64_t x)
{
    /* Factorisation.  factor_u64 returns primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64(x, primes, exps, &pcnt);

    /* Generate all divisors.  dsize is the current number of divisors
       stored in the array 'divs'. */
    uint64_t divs[2048];
    size_t dsize = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        size_t prev = dsize;         /* divisors before adding new powers */
        uint64_t pow = 1;
        for (size_t e = 1; e <= exps[i]; ++e) {
            pow *= primes[i];
            for (size_t j = 0; j < prev; ++j)
                divs[dsize++] = divs[j] * pow;
        }
    }

    /* All divisors must be odious.  If any fails, return false. */
    for (size_t i = 0; i < dsize; ++i)
        if (!odious_uint64(divs[i])) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* provide a value for n<1 */

    /* Static cache of computed terms.  cache[0] = a(1).  The cache
       grows as needed but is independent of the order of calls. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  size  = 0;                   /* number of terms stored */

    /* Allocate initial cache if first call. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Grow cache to accommodate required index. */
    while (size < n) {
        if (size == 0) {                      /* first term is 1 */
            cache[size++] = (seq_t)1;
            if (size == n) break;
        }

        /* Find next odd candidate.  Start at 3 after the first term. */
        static seq_t next_candidate = 3;
        bool found = false;

        while (!found) {
            uint64_t cand = (uint64_t)next_candidate;
            next_candidate += (seq_t)2;       /* only odd numbers */

            if (!odious_uint64(cand)) continue;  /* candidate must be odious */

            if (all_divisors_odious(cand)) {
                if (size == cap) {           /* grow until capacity */
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                }
                cache[size++] = (seq_t)cand;
                found = true;
            }
        }
    }

    return cache[n - 1];
}
