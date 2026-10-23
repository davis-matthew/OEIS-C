/*
 * A192087  Potential magic constants of a 10 X 10 magic square composed of
 *          consecutive primes.
 * https://oeis.org/A192087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A192087 – Potential magic constants of a 10 × 10 magic square composed of consecutive primes
    Formula: a(n) = (sum of 100 consecutive primes starting from the n‑th qualifying block) / 10;
    A block qualifies if the sum of its 100 primes is a multiple of 20. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/*--- static state ----------------------------------------------*/
static idx_t current_limit = 3000000;      /* sieve size used for primes */
static uint64_t *prime_list = NULL;       /* array of primes */
static size_t    prime_count = 0;         /* number of primes produced */
static seq_t *prime_prefix = NULL;        /* prefix sums of primes, size prime_count+1 */

static seq_t *term_cache = NULL;          /* cache of a(1), a(2), … */
static size_t term_len = 0;               /* number of cached terms */
static size_t term_cap = 0;               /* capacity of term_cache */

static idx_t next_start = 0;              /* start index of next 100‑prime block to test */

static bool cache_initialized = false;    /* true after first call to a() */

/*---- helper: rebuild the prime list and its prefix sums -------------*/
static void rebuild_primes(idx_t new_limit)
{
    free(prime_list);
    free(prime_prefix);

    build_primes_upto(new_limit, &prime_list, &prime_count);

    prime_prefix = (seq_t *)malloc((prime_count + 1) * sizeof(seq_t));
    if (!prime_prefix) abort();
    prime_prefix[0] = (seq_t)0;
    for (size_t i = 1; i <= prime_count; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + (seq_t)prime_list[i - 1];
    }
}

/*--------------------------- main routine -----------------------------*/
seq_t a(idx_t n)
{
    if (!cache_initialized) {
        rebuild_primes(current_limit);
        cache_initialized = true;
    }

    if (n < 1) return (seq_t)0;   /* undefined behaviour for n<1 */

    size_t target = (size_t)n;

    /* generate terms until we have at least 'target' terms cached */
    while (term_len < target) {
        /* ensure enough primes for the next 100‑prime window */
        if ((size_t)(next_start + 100) > prime_count) {
            current_limit *= 2;            /* enlarge sieve limit */
            rebuild_primes(current_limit);
            continue;                     /* retry with expanded prime list */
        }

        /* compute sum of primes from next_start to next_start+99 */
        seq_t sum = prime_prefix[(size_t)(next_start + 100)]
                  - prime_prefix[(size_t)next_start];

        if (sum % (seq_t)20 == 0) {         /* qualifying block */
            seq_t val = sum / (seq_t)10;

            if (term_len == term_cap) {     /* grow cache if needed */
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                term_cache = tmp;
                term_cap = new_cap;
            }

            term_cache[term_len++] = val;
        }

        ++next_start;                       /* move to next block */
    }

    return term_cache[(size_t)(n - 1)];
}
