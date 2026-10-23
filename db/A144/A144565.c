/*
 * A144565  a(n) is the smallest composite number > a(n-1) that contains the
 *          n-th prime as a substring.
 * https://oeis.org/A144565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes, isPrimeTrial, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144565:
   a(n) is the smallest composite number > a(n-1) that contains the n‑th prime
   as a decimal substring.  Offset 1 (a(1) is the first term).  The algorithm
   searches forward from a(n-1)+1, skipping primes, and stops when a composite
   containing the required prime string is found.  The primes are generated on
   demand, and the sequence values are memoised for repeated queries. */

/* Utility headers (available from the build system) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/build_primes.h"
#include "../utils/seq_to_str.h"

static seq_t *cache = NULL;          /* cache[i] holds a(i+1)           */
static size_t cache_len = 0;         /* number of computed terms        */
static size_t cache_cap = 0;         /* allocated capacity of cache     */

static idx_t *prime_list = NULL;     /* array of primes (idx_t type)    */
static size_t prime_cnt = 0;         /* number of primes stored         */
static idx_t  prime_limit = 0;       /* current sieve limit             */

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    if (prime_limit == 0)
        prime_limit = 20;        /* small initial limit */

    while (prime_cnt < need) {
        prime_limit *= 2;                      /* grow the limit           */
        idx_t *new_list = NULL;
        size_t new_cnt  = 0;

        /* build_primes allocates a new array of idx_t primes */
        build_primes(prime_limit, &new_list, &new_cnt);

        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
    }
}

/* Grow cache capacity to accommodate 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort as the sequence cannot be computed */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* initialise prime list on first call */
    if (prime_list == NULL)
        ensure_primes(1);           /* at least the first prime */

    /* compute missing terms up to n */
    while (cache_len < n) {
        seq_t prev = (cache_len == 0) ? 0 : cache[cache_len - 1];

        /* prime that must appear in the decimal representation */
        ensure_primes(cache_len + 1);   /* cache_len is 0‑based index */
        idx_t needed_prime = prime_list[cache_len];

        /* convert prime to string */
        char prime_str[32];
        snprintf(prime_str, sizeof prime_str, "%llu",
                 (unsigned long long)needed_prime);

        /* search for the smallest composite containing the prime */
        idx_t cand = (idx_t)(prev) + 1;   /* start from a(n-1)+1 */
        for (;;) {
            /* candidate > 1 and composite */
            if (cand > 1 && !isPrimeTrial((seq_t)cand)) {
                /* convert cand to string for substring test */
                char cand_buf[64];
                size_t cand_len = 0;
                seq_to_str((seq_t)cand, cand_buf, &cand_len);
                cand_buf[cand_len] = '\0';   /* null‑terminate */

                if (strstr(cand_buf, prime_str) != NULL)
                    break;   /* found the required composite */
            }
            cand = cand + 1;
        }

        /* store the new term */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = (seq_t)cand;
        ++cache_len;
    }

    return cache[n - 1];
}
