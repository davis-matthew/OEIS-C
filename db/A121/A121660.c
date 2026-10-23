/*
 * A121660  Duplicate of A118835.
 * https://oeis.org/A121660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   enlarge_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  a(n) = numerator of the continued fraction  
 *  [4, 6, 9, …, A001358(n)]  (the first n semiprimes)
 *  Where A001358(n) is the n‑th semiprime.
 *  The recurrence for the numerators is  
 *          a(1) = 4
 *          a(k) = semiprime[k] * a(k‑1) + a(k‑2)   (k ≥ 2)
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/enlarge_seq_array.h"

/* ----------  Static state  ---------- */
static seq_t *semiprimes     = NULL;   /* array of semiprimes */
static idx_t  semip_count    = 0;      /* entries stored */
static idx_t  semip_cap      = 0;      /* allocated capacity */
static uint64_t semip_last   = 1;      /* last number checked for semiprime */

static uint64_t *spf          = NULL;  /* smallest prime factor array */
static uint64_t  spf_limit     = 0;     /* current spf array size */

static seq_t *cache          = NULL;   /* computed numerators  */
static idx_t  cache_max      = 0;      /* largest n cached (1‑based)*/
static idx_t  cache_cap      = 0;      /* allocated capacity */

/* ----------  Helper: sieve smallest prime factor up to limit ---------- */
static void build_spf(uint64_t limit)
{
    if (limit <= spf_limit) return;           /* already computed up to limit */
    /* re‑allocate and zero-initialize */
    spf = (uint64_t *)realloc(spf, (limit + 1) * sizeof(uint64_t));
    if (!spf) abort();
    for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (uint64_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (uint64_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = i;
    spf_limit = limit;
}

/* ----------  Helper: test if m is a semiprime ---------- */
static int is_semiprime(uint64_t m)
{
    int cnt = 0;
    while (m > 1 && cnt <= 2) {
        uint64_t p = spf[m];
        m /= p;
        ++cnt;
    }
    return cnt == 2;
}

/* ----------  Ensure we have at least need semiprimes ---------- */
static void ensure_semiprimes(idx_t need)
{
    if (need <= semip_count) return;

    uint64_t limit = spf_limit ? spf_limit : (need * 20 + 100);
    if (limit < 100) limit = 100;

    /* Grow until we have enough semiprimes */
    while (1) {
        build_spf(limit);

        /* Scan numbers from last_checked+1 to limit */
        for (uint64_t num = semip_last + 1; num <= limit; ++num) {
            if (is_semiprime(num)) {
                enlarge_seq_array(&semiprimes, &semip_cap, semip_count + 1);
                semiprimes[semip_count++] = (seq_t)num;
                if (semip_count >= need) {
                    semip_last = num;
                    return;
                }
            }
        }
        /* Not enough; double the limit and try again */
        semip_last = limit;
        limit <<= 1;
    }
}

/* ----------  Ensure we have a(n) computed for all n up to need ---------- */
static void ensure_numerators(idx_t need)
{
    if (need <= cache_max) return;

    /* Grow cache if necessary */
    enlarge_seq_array(&cache, &cache_cap, need);

    /* Calculate from scratch if cache empty */
    if (cache_max == 0) {
        cache[0] = semiprimes[0];        /* a(1) */
        cache_max = 1;
    }

    /* Fill in successive values */
    seq_t prevprev = (cache_max == 1) ? 1 : cache[cache_max - 2];
    seq_t prev    = cache[cache_max - 1];

    for (idx_t i = cache_max + 1; i <= need; ++i) {
        seq_t next = semiprimes[i - 1] * prev + prevprev;
        cache[i - 1] = next;
        prevprev = prev;
        prev = next;
    }
    cache_max = need;
}

/* ----------  Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but defensive */
    ensure_semiprimes(n);
    ensure_numerators(n);
    return cache[n - 1];
}
