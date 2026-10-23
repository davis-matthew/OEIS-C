/*
 * A165559  Product of the arithmetic derivatives from 2 to n.
 * https://oeis.org/A165559
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \prod_{k=2..n} A003415(k) (product of the arithmetic
 *            derivatives of all integers from 2 to n)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

/* Static cache of already computed a(k) values.  The array is indexed
   by k, with prod_cache[1] = 1 as the empty product.  prod_cache[n] holds
   a(n) for each n ≥ 2. */
static seq_t      *prod_cache = NULL;
static size_t      prod_cap   = 0;
static idx_t       last_n     = 0;   /* largest n for which prod_cache[n] is valid */

/* Cache of smallest prime factors, used by arithmetic_derivative. */
static uint64_t    *spf_cache = NULL;
static size_t      spf_cap   = 0;

/* Ensure spf_cache contains smallest prime factor for every integer ≤ n. */
static void ensure_spf(idx_t n)
{
    if (spf_cap >= n + 1) return;     /* already enough */
    if (spf_cache) free(spf_cache);   /* discard old data */
    build_spf(n, &spf_cache);         /* generate new array */
    spf_cap = n + 1;
}

/* Ensure prod_cache is large enough to store indices up to n. */
static void ensure_prod_cache(idx_t n)
{
    if (prod_cap >= n + 1) return;     /* already large enough */

    seq_t *new_arr = seq_realloc(prod_cache, prod_cap, n + 1);
    /* seq_realloc returns a newly allocated array with the old data copied.
       New entries (if any) are uninitialized; they will be filled later. */
    prod_cache = new_arr;
    prod_cap   = n + 1;

    /* Initialise base case on first use. */
    if (last_n == 0) {
        prod_cache[1] = 1;            /* product over empty set */
        last_n = 1;
    }
}

/* Arithmetic derivative of an integer k (k ≥ 2).  Uses spf_cache. */
static inline seq_t arithmetic_derivative(idx_t k)
{
    if (k < 2) return 0;        /* D(0) = D(1) = 0 */

    seq_t duk = 0;
    idx_t tmp = k;
    seq_t knum = (seq_t)k;      /* original value of k */

    while (tmp > 1) {
        idx_t p = spf_cache[tmp];
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        duk += (knum / p) * (seq_t)e;   /* term k * e / p */
    }
    return duk;
}

/* Main sequence function.  Computes a(n) for any n ≥ 2, in any order. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for indices below offset */

    ensure_spf(n);
    ensure_prod_cache(n);

    /* Compute forward from last_n+1 up to n (inclusive). */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        seq_t fk = arithmetic_derivative(k);
        prod_cache[k] = prod_cache[k - 1] * fk;
    }
    last_n = n;

    return prod_cache[n];
}
