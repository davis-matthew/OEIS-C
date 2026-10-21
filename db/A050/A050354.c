/*
 * A050354  Number of ordered factorizations of n with one level of
 *          parentheses.
 * https://oeis.org/A050354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*∑_{d|n, d<n} a(d) – 1 (with a(1) = 1) Recurrence
 *            obtained from the Dirichlet generating function (2 – ζ(s))/(3 –
 *            2ζ(s)) implemented by dynamic programming with divisor sums
 * Range:     u8 n <= 47, u16 n <= 1151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array */
#include "../utils/seq_realloc.h"           /* seq_realloc */
#include "../utils/build_spf.h"             /* build_spf */

static seq_t *cache = NULL;          /* cache of sequence values, 1‑based */
static size_t cache_cap = 0;         /* current allocated cache size */
static size_t cache_len = 0;         /* last computed index (a(cache_len) is known) */

static idx_t *spf = NULL;            /* smallest prime factor array */
static size_t spf_len = 0;           /* current size of spf */

/* Ensure that spf array covers at least [1..n] */
static void ensure_spf(idx_t n)
{
    if (spf_len >= n) return;
    if (spf != NULL) free(spf);
    build_spf((idx_t)n, &spf);
    spf_len = (size_t)n;
}

/* Ensure cache covers indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Factorise n using spf, return number of distinct prime factors.
   primes[] and exps[] are of size at least 20. */
static int factorise(idx_t n, idx_t primes[], int exps[])
{
    int cnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }
    return cnt;
}

/* Recursively generate all proper divisors of n.  The global arrays
   'primes' and 'exps' carry the factorisation of n; m is the count.
   The array divs is filled starting at index 0; *cnt records length. */
static void gen_divs_rec(const idx_t *primes, const int *exps,
                        int idx, int m, idx_t curr, idx_t divs[],
                        int *cnt, const idx_t n)
{
    if (idx == m) {
        if (curr != n) divs[(*cnt)++] = curr;
        return;
    }
    idx_t pp = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(primes, exps, idx + 1, m, curr * pp,
                     divs, cnt, n);
        pp *= primes[idx];
    }
}

static void compute_divisors(idx_t n, idx_t divs[], int *cnt)
{
    idx_t primes[20];
    int exps[20];
    int m = factorise(n, primes, exps);
    *cnt = 0;
    gen_divs_rec(primes, exps, 0, m, 1, divs, cnt, n);
}

/*-------------------  public function  -------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined, but avoid crash */

    /* Expand cache and spf if necessary */
    ensure_cache(n);
    ensure_spf(n);

    /* Compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else {
            idx_t divs[128];
            int divcnt;
            compute_divisors(i, divs, &divcnt);
            seq_t sum = 0;
            for (int j = 0; j < divcnt; ++j) {
                sum += cache[divs[j] - 1];   /* divs[j] < i guarantees cache ready */
            }
            cache[i - 1] = 2 * sum - 1;
        }
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
