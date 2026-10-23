/*
 * A141632  Nonprimes of the form c(c(c(c(i)-3)-3)-3)-3, where
 *          A002808(i)=c(i)=i-th composite.
 * https://oeis.org/A141632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th nonprime value of * f(i) = c(c(c(c(i) - 3) - 3)
 *            - 3) - 3, * where c(k) is the k‑th composite number (A002808)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* ---------- internal helpers ------------------------------------- */

static seq_t *composites = NULL;          /* array of composite numbers */
static size_t   comp_size  = 0;           /* number of composites stored    */
static idx_t    sieve_limit = 0;          /* last limit used for sieve      */

static void ensure_composites(idx_t target)
{
    if (comp_size >= (size_t)target)
        return;

    idx_t lim = sieve_limit ? sieve_limit * 2 : 1024;
    while (1) {
        /* sieve up to lim */
        unsigned char *is_prime = (unsigned char*)calloc((size_t)lim + 1, 1);
        if (!is_prime) return;                /* out of memory */
        for (idx_t i = 2; i <= lim; ++i)
            is_prime[i] = 1;
        for (idx_t p = 2; p * p <= lim; ++p) {
            if (!is_prime[p]) continue;
            for (idx_t j = p * p; j <= lim; j += p)
                is_prime[j] = 0;
        }
        /* count composites (>=4) */
        size_t cnt = 0;
        for (idx_t n = 4; n <= lim; ++n)
            if (!is_prime[n]) ++cnt;
        seq_t *new_comps = (seq_t*)malloc(cnt * sizeof(seq_t));
        if (!new_comps) {
            free(is_prime);
            return;                            /* out of memory */
        }
        size_t pos = 0;
        for (idx_t n = 4; n <= lim; ++n)
            if (!is_prime[n]) new_comps[pos++] = (seq_t)n;
        free(is_prime);
        free(composites);
        composites = new_comps;
        comp_size  = cnt;
        sieve_limit = lim;
        if (comp_size >= (size_t)target)
            return;
        lim *= 2;                               /* increase limit */
    }
}

static seq_t compute_f(idx_t i)
{
    seq_t t = i;
    for (int step = 0; step < 4; ++step) {
        ensure_composites((idx_t)t);
        t = composites[(idx_t)t - 1] - 3;
    }
    return t;
}

/* ---------- public function ------------------------------------- */

static seq_t *cache   = NULL;            /* memoised a(n) values   */
static size_t cache_size = 0;            /* how many computed    */
static size_t cache_cap  = 0;            /* allocated capacity   */
static idx_t  last_i = 0;                /* last i processed     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, defensive */
    while (cache_size < (size_t)n) {
        ++last_i;
        seq_t val = compute_f(last_i);
        if (!is_prime_big(val)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = val;
        }
    }
    return cache[n - 1];
}
