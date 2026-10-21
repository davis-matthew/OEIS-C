/*
 * A077127  a(0) = 1, a(1) = 2, a(n) = smallest number other than n,
 *          non-coprime to n and not occurring earlier.
 * https://oeis.org/A077127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077127: a(0)=1, a(1)=2, a(n)=smallest m≠n,
   non‑coprime to n, and not used earlier.  The sequence is a
   permutation of the natural numbers.  This implementation
   precomputes a small sieve of primes and maintains for each
   prime p the smallest unused multiple of p.  For each index n
   we factor n, find the minimal candidate among its prime
   factors, then update the bookkeeping data structures. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static void init_sequence(void);
static void factor_unique(idx_t n, uint32_t *pr, size_t *cnt);

static bool initialized = false;
static seq_t *buf = NULL;               /* computed sequence values      */
static size_t buf_len = 0, buf_cap = 0;  /* buffer capacity              */

static uint8_t *used = NULL;            /* bitmap of used values */
static size_t used_cap = 0;

static uint32_t *next_mul = NULL;       /* smallest unused multiple per prime */
static uint32_t *spf = NULL;            /* smallest prime factor for each n  */
static size_t sieve_limit = 0;          /* size of the sieve arrays           */

static const size_t INIT_BUF_CAP = 16;
static const size_t INIT_SIEVE = 1000000;     /* large enough for n ≤ 1e4       */

static void init_sequence(void)
{
    /* Allocate memory for the sieve and associated arrays */
    sieve_limit = INIT_SIEVE;
    used_cap   = sieve_limit + 1;
    used       = (uint8_t*)calloc(used_cap, 1);
    next_mul   = (uint32_t*)malloc(sizeof(uint32_t)*(sieve_limit+1));
    spf        = (uint32_t*)malloc(sizeof(uint32_t)*(sieve_limit+1));

    /* Simple linear sieve for SPF */
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (size_t j = i*i; j <= sieve_limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (size_t i = 0; i <= sieve_limit; ++i) {
        if (spf[i] == i && i >= 2)   /* prime */
            next_mul[i] = (uint32_t)i;
        else
            next_mul[i] = 0;
    }

    /* Allocate buffer */
    buf_cap = INIT_BUF_CAP;
    buf = (seq_t*)malloc(buf_cap * sizeof(seq_t));
    buf_len = 0;

    /* seed first two terms */
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)2;
    buf_len = 2;
    if (used_cap < 3) {
        size_t new_cap = used_cap * 2;
        while (new_cap < 3) new_cap *= 2;
        used = (uint8_t*)realloc(used, new_cap);
        memset(used + used_cap, 0, new_cap - used_cap);
        used_cap = new_cap;
    }
    used[1] = 1;
    used[2] = 1;

    initialized = true;
}

/* Factor n into unique primes; max primes ≤ 32 */
static void factor_unique(idx_t n, uint32_t *pr, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        pr[(*cnt)++] = p;
        while (n % p == 0) n /= p;
    }
}

/* Main function to compute A077127 */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* Compute terms up to index n */
    while (buf_len <= (size_t)n) {
        idx_t i = (idx_t)buf_len;              /* next index to fill */

        /* Factor i */
        uint32_t primes_i[32];
        size_t cnt_i = 0;
        factor_unique(i, primes_i, &cnt_i);

        /* Find minimal candidate */
        uint32_t best = 0xFFFFFFFF;
        for (size_t k = 0; k < cnt_i; ++k) {
            uint32_t p = primes_i[k];
            uint32_t cand = next_mul[p];

            /* Ensure cand is not equal to i and unused */
            if (cand == ((uint32_t)i)) cand += p;
            while (cand <= (uint32_t)used_cap &&
                   (cand == ((uint32_t)i) || used[cand])) {
                cand += p;
            }
            if (cand < best) best = cand;
        }

        /* If no candidate found within current capacity, grow arrays. */
        if (best == 0xFFFFFFFF) {
            /* Increase sieve size to accommodate larger values */
            size_t new_limit = sieve_limit * 2;
            used = (uint8_t*)realloc(used, new_limit + 1);
            memset(used + used_cap, 0, new_limit - used_cap + 1);
            used_cap = new_limit + 1;

            next_mul = (uint32_t*)realloc(next_mul,
                sizeof(uint32_t) * (new_limit + 1));
            spf = (uint32_t*)realloc(spf,
                sizeof(uint32_t) * (new_limit + 1));

            /* Extend sieve and SPF */
            for (size_t j = sieve_limit + 1; j <= new_limit; ++j) {
                spf[j] = 0;
            }
            for (size_t p = 2; p <= new_limit; ++p) {
                if (spf[p] == 0) {
                    spf[p] = (uint32_t)p;
                    for (size_t j = p * p; j <= new_limit; j += p) {
                        if (spf[j] == 0) spf[j] = (uint32_t)p;
                    }
                }
            }
            for (size_t j = 2; j <= new_limit; ++j) {
                if (spf[j] == j) next_mul[j] = (uint32_t)j;
                else next_mul[j] = 0;
            }
            sieve_limit = new_limit;
            /* Retry for current i after expansion */
            continue;
        }

        /* Mark best as used */
        if (best >= used_cap) {
            /* Expand used array if necessary */
            size_t new_cap = used_cap * 2;
            while (new_cap <= (size_t)best) new_cap *= 2;
            used = (uint8_t*)realloc(used, new_cap);
            memset(used + used_cap, 0, new_cap - used_cap);
            used_cap = new_cap;
        }
        used[best] = 1;

        /* Update next_mul for primes dividing best */
        uint32_t primes_best[32];
        size_t cnt_best = 0;
        factor_unique(best, primes_best, &cnt_best);
        for (size_t k = 0; k < cnt_best; ++k) {
            uint32_t q = primes_best[k];
            uint32_t cand = next_mul[q];
            /* If cand is zero, initialize to smallest multiple */
            if (cand == 0) cand = q;
            while (cand <= (uint32_t)used_cap && used[cand]) {
                cand += q;
            }
            next_mul[q] = cand;
        }

        /* Store result */
        if (buf_len == buf_cap) {
            buf_cap *= 2;
            buf = (seq_t*)realloc(buf, buf_cap * sizeof(seq_t));
        }
        buf[buf_len++] = (seq_t)best;
    }
    return buf[n];
}
