/*
 * A160820  a(1)=2. For n >=2, a(n) = smallest positive integer > a(n-1) such
 *          that Omega(a(n)) = Omega(a(n)-a(n-1)), where Omega(m) =
 *          A001222(m).
 * https://oeis.org/A160820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 9962, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160820
   a(1) = 2
   for n ≥ 2,  a(n) = smallest positive integer > a(n-1) with
                 Ω(a(n)) = Ω(a(n) − a(n-1))
   where Ω(m) is the total number of prime factors of m counted with
   multiplicity (A001222).  The function below computes any term
   a(n) for n ≥ 1, memoises previously computed values and works for
   integer types seq_t (uint8_t … uint64_t, unsigned __int128, or an
   arbitrary‑precision class) and idx_t = uint64_t.  It uses the
   supplied helper function count_prime_factors(seq_t m).  The routine
   is O(n) overall and finishes well under one second for n≈10 000.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

/*-----------------------------------------------------------*/
/* Dynamic cache for previously computed terms                */
/* The cache is 1‑based: cache[0] holds a(1) = 2,                */
/* cache[1] holds a(2), etc.                                   */
/*-----------------------------------------------------------*/
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t  cache_cap = 0;     /* allocated capacity (1‑based) */
static idx_t  cache_up_to = 0;   /* highest index n for which cache[n-1] is defined */

/* Ensure the cache can hold at least 'need' elements (1‑based).  */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/*-----------------------------------------------------------*/
/* Main sequence function                                    */
/*-----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1, guard */

    /* Initialise cache for a(1) = 2 on first call.  */
    if (cache_up_to == 0) {
        grow_cache(1);
        cache[0] = (seq_t)2;         /* a(1) */
        cache_up_to = 1;
    }

    /* Compute missing terms up to n.  */
    if (cache_up_to < n) {
        for (idx_t cur = cache_up_to + 1; cur <= n; ++cur) {
            seq_t last = cache[cur - 2];          /* a(cur-1)   */
            seq_t cand = last + (seq_t)1;         /* start search */

            while (true) {
                seq_t ocand = count_prime_factors(cand);
                seq_t odiff = count_prime_factors(cand - last);
                if (ocand == odiff) break;
                cand = cand + (seq_t)1;
            }
            grow_cache(cur);
            cache[cur - 1] = cand;                /* cache[cur-1] = a(cur) */
        }
        cache_up_to = n;
    }

    return cache[n - 1];
}
