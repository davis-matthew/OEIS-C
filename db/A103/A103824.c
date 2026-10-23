/*
 * A103824  Choose a(n) to be the smallest number not yet used such that: a(1)
 *          = 1, a(2*n) = prime, a(2*n+1) = composite and partial sums are
 *          alternately composite or prime.
 * https://oeis.org/A103824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A103824 – constructive generation
 *
 *  a(1) = 1
 *  For n > 1 :
 *  • if n is even, a(n) must be prime
 *  • if n is odd,  a(n) must be composite
 *  The partial sums s(n)=∑_{k≤n}a(k) satisfy
 *  • s(n) is prime for odd n, composite for even n
 *
 *  The function below generates the sequence lazily, memoising already
 *  computed terms.  It works for all supported seq_t types, including
 *  arbitrary‑precision integer classes used in the C++ build.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* ---------- internal helper ----------------------------------------- */
static inline bool is_prime(seq_t x)
{
    return is_prime_seq(x);
}

/* ---------- static state -------------------------------------------- */
static seq_t *cache = NULL;                /* cache[0] unused; cache[1]==1 */
static size_t cache_len = 0;               /* number of stored elements   */
static unsigned char *used = NULL;         /* used[i] == 1 if number i used */
static size_t used_len = 0;
static seq_t cur_sum = 0;                  /* sum of terms so far        */
static bool initialized = false;

/* ---------- initialise static state --------------------------------- */
static void init_static(void)
{
    if (initialized) return;

    /* initialise cache with a(1) */
    cache_len = 2;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    cache[0] = (seq_t)0;                   /* dummy */
    cache[1] = (seq_t)1;
    cur_sum = (seq_t)1;

    /* initialise used array */
    used_len = 2;
    used = (unsigned char *)calloc(used_len, 1);  /* zeroed */

    /* mark 1 as used */
    used[1] = 1;

    initialized = true;
}

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    init_static();

    /* already computed? */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* extend cache up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        uint64_t cand = 2;                   /* start searching at 2 */

        /* search smallest candidate satisfying constraints */
        for ( ;; ++cand ) {
            /* ensure used array is large enough */
            if (cand >= used_len) {
                size_t new_len = cand + 1;
                unsigned char *tmp = (unsigned char *)realloc(used, new_len);
                if (!tmp) abort();           /* memory exhausted */
                /* initialise newly appended bytes to 0 */
                memset(tmp + used_len, 0, new_len - used_len);
                used = tmp;
                used_len = new_len;
            }
            if (used[(size_t)cand]) continue;

            /* parity condition on a(i) */
            if ( (i & 1ULL) == 0ULL ) {        /* even index → prime   */
                if (!is_prime((seq_t)cand)) continue;
            } else {                          /* odd index  → composite */
                if ((cand < 4ULL) || is_prime((seq_t)cand))
                    continue;
            }

            /* partial sum condition */
            seq_t new_sum = cur_sum + (seq_t)cand;
            if ( (i & 1ULL) == 0ULL ) {        /* even sum → composite */
                if (is_prime(new_sum)) continue;
            } else {                          /* odd sum  → prime    */
                if (!is_prime(new_sum)) continue;
            }

            /* candidate accepted */
            break;
        }

        /* record the chosen value */
        seq_t val = (seq_t)cand;

        /* expand cache if needed */
        if (i >= cache_len) {
            size_t new_len = i + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();               /* memory exhausted */
            cache = tmp;
            cache_len = new_len;
        }

        cache[i] = val;
        used[(size_t)cand] = 1;
        cur_sum += val;
    }

    return cache[(size_t)n];
}
