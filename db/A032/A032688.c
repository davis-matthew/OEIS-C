/*
 * A032688  Numbers k such that k surrounded by digit '5' is a lucky number.
 * https://oeis.org/A032688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 1381, u64 n <= 1381,
 *            u128 n <= 1381, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: this function generates the sequence A032688, i.e. the
 * positive integers k such that 5k5 is a lucky number.  The algorithm
 * builds the list of lucky numbers up to the largest 5k5 it has to
 * inspect, enlarging it on demand.  Each new term is found by
 * testing successive k until 5k5 appears in that list.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

/* --- global data ----------------------------------------------------- */
static seq_t *lucky = NULL;            /* current list of lucky numbers          */
static size_t lucky_len = 0;           /* number of lucky numbers                */
static uint32_t lucky_limit = 0;       /* sieve limit (max value included)       */

static seq_t *cache = NULL;            /* memoised a(n) values                  */
static size_t cache_len = 0;           /* number of cached solutions            */
static size_t cache_cap = 0;           /* allocated capacity for cache           */
static idx_t   current_k = 1;          /* next k to test                        */

/* --------------------------------------------------------------------- */
static void generate_lucky_vector(uint32_t limit)
{
    /* free previous data, if any */
    if (lucky) {
        free(lucky);
        lucky = NULL;
    }

    lucky_limit = limit;

    /* array of odd numbers up to limit */
    size_t maxOdds = limit / 2 + (limit & 1);
    uint32_t *src = (uint32_t *)malloc(maxOdds * sizeof(uint32_t));
    uint32_t *dst = (uint32_t *)malloc(maxOdds * sizeof(uint32_t));
    if (!src || !dst) {
        exit(1);
    }

    for (size_t i = 0; i < maxOdds; ++i)
        src[i] = 1 + (uint32_t)(2 * i);

    size_t sz   = maxOdds;
    size_t idx  = 1;                       /* skip 1, it is not used as step */

    while (idx < sz) {
        uint32_t step = src[idx];
        if (step > sz)                     /* no more eliminations possible   */
            break;

        size_t dstpos = 0;
        for (size_t j = 0; j < sz; ++j)
            if (((j + 1) % step) != 0)
                dst[dstpos++] = src[j];

        sz = dstpos;
        /* swap source and destination */
        uint32_t *tmp = src;
        src = dst;
        dst = tmp;
        ++idx;
    }

    /* store result as seq_t array */
    lucky_len = sz;
    lucky = (seq_t *)malloc(lucky_len * sizeof(seq_t));
    if (!lucky) {
        exit(1);
    }
    for (size_t i = 0; i < lucky_len; ++i)
        lucky[i] = (seq_t)src[i];

    free(src);
    free(dst);
}

/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    /* lazy generation of lucky list (initial limit 1 M) */
    if (!lucky)
        generate_lucky_vector(1000000U);

    while (cache_len < (size_t)n) {
        /* compute 5k5 for current_k */
        int d = digits(current_k);                       /* digits of k           */
        seq_t pow = pow10((idx_t)(d + 1));                /* 10^(d+1)             */
        seq_t val = (seq_t)5 * pow + (seq_t)current_k * (seq_t)10 + (seq_t)5;

        /* enlarge lucky list if necessary */
        if ((uint64_t)val > (uint64_t)lucky_limit) {
            uint32_t new_limit = lucky_limit ? lucky_limit : 1000000U;
            /* double until it covers val or reaches UINT32_MAX */
            uint64_t val_u64 = (uint64_t)val;
            while ((uint64_t)new_limit < val_u64 && new_limit < UINT32_MAX/2)
                new_limit <<= 1;
            if ((uint64_t)new_limit < val_u64)
                new_limit = UINT32_MAX;
            generate_lucky_vector(new_limit);
        }

        /* test presence */
        bool is_lucky = false;
        size_t lo = 0, hi = lucky_len;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (lucky[mid] < val)
                lo = mid + 1;
            else if (lucky[mid] > val)
                hi = mid;
            else {
                is_lucky = true;
                break;
            }
        }

        if (is_lucky) {
            /* append to cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_k;
        }

        ++current_k;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 */

    ensure_cache(n);
    return cache[n - 1];
}
