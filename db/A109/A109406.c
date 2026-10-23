/*
 * A109406  Numbers m such that (sp(m)+sp(m+1)+sp(m+2))/3 is an integer, where
 *          sp(n) = n-th semiprime.
 * https://oeis.org/A109406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that (sp(m)+sp(m+1)+sp(m+2))/3 is
 *            integer
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   cmp_seq, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here sp(k) denotes the k-th semiprime. */

/* This implementation keeps a global list of all semiprimes that have been
   generated so far (in ascending order) and a global list of all indices
   m that satisfy the divisibility condition.  For each new request it
   expands the lists until the requested index N is available.  The
   registration keyword ensures that the sequence can be requested in
   arbitrary order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "../utils/simple_sieve.h"   /* simple_sieve(limit,&primes,&cnt) */
#include "../utils/cmp_seq.h"        /* cmp_seq(const void*,const void*) */

static seq_t *semiprimes = NULL;               /* sorted semiprime values   */
static size_t semiprime_count = 0;             /* number of stored semiprimes */
static idx_t   current_limit   = 0;            /* current max value for semiprimes */

static idx_t *good_ms = NULL;                  /* indices m satisfying the test */
static size_t good_cap    = 0;                 /* capacity of good_ms         */
static size_t good_count  = 0;                 /* number of stored good m's   */

static idx_t last_checked = 0;                 /* highest m index already checked */

/* -------------------------------------------------------------------------- */
/* Dynamic array helpers for idx_t                                         */
/* -------------------------------------------------------------------------- */
static void add_good_m(idx_t val) {
    if (good_count == good_cap) {
        size_t newcap = (good_cap == 0) ? 32 : good_cap << 1;
        idx_t *tmp = (idx_t *)realloc(good_ms, newcap * sizeof(idx_t));
        assert(tmp);
        good_ms = tmp;
        good_cap = newcap;
    }
    good_ms[good_count++] = val;
}

/* -------------------------------------------------------------------------- */
/* Generate all semiprimes <= limit.  Replaces the previous list.           */
/* -------------------------------------------------------------------------- */
static void regenerate_semiprimes(idx_t limit) {
    if (limit <= current_limit) return;   /* nothing to do */

    /* Get all primes up to 'limit' */
    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    simple_sieve((uint32_t)limit, &primes, &primes_cnt);

    /* Quick estimate of number of semiprimes to allocate */
    size_t estimated = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t prod = (uint64_t)primes[i] * (uint64_t)primes[j];
            if (prod > (uint64_t)limit) break;
            ++estimated;
        }
    }

    /* Allocate buffer and generate semiprimes */
    seq_t *buf = (seq_t *)malloc(estimated * sizeof(seq_t));
    assert(buf);
    size_t idx = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t prod = (uint64_t)primes[i] * (uint64_t)primes[j];
            if (prod > (uint64_t)limit) break;
            buf[idx++] = (seq_t)prod;
        }
    }

    /* Sort to maintain sp(n) order */
    qsort(buf, idx, sizeof(seq_t), cmp_seq);

    /* Replace old list */
    free(semiprimes);
    semiprimes = buf;
    semiprime_count = idx;
    current_limit = limit;

    free(primes);
}

/* -------------------------------------------------------------------------- */
/* Ensure we have evaluated all required m up to the n‑th good value.         */
/* -------------------------------------------------------------------------- */
static void ensure_good_ms(size_t needed) {
    if (semiprimes == NULL) {
        regenerate_semiprimes(1000);  /* initial small list */
    }

    while (good_count < needed) {
        /* Ensure we have at least last_checked+3 semiprimes for the test */
        while (semiprime_count < (size_t)(last_checked + 3)) {
            idx_t new_limit = current_limit ? current_limit * 2 : 1000;
            regenerate_semiprimes(new_limit);
        }

        /* Test current m */
        seq_t s1 = semiprimes[last_checked];
        seq_t s2 = semiprimes[last_checked + 1];
        seq_t s3 = semiprimes[last_checked + 2];
        seq_t sum = s1 + s2 + s3;
        if ((sum % (seq_t)3) == (seq_t)0) {
            add_good_m(last_checked + 1);   /* 1‑based index */
        }
        ++last_checked;
    }
}

/* -------------------------------------------------------------------------- */
/* Required external function                                            */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (good_ms == NULL) {
        good_ms = NULL;    /* will be allocated by add_good_m */
    }
    ensure_good_ms((size_t)n);
    return (seq_t)good_ms[n - 1];
}
