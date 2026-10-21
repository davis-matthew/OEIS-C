/*
 * A211432  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2=x^2+y^2.
 * https://oeis.org/A211432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 8 * Σ_{r=1..n} Π_{p≡1 (mod 4)} (2·exp_p(r) + 1)
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where exp_p(r) is the exponent of prime p in r) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state */
static idx_t      memo_max = 0;     /* largest n for which prefix is valid   */
static idx_t     *spf      = NULL;  /* smallest prime factor array           */
static seq_t    *prefix   = NULL;   /* prefix sums: prefix[n] = a(n)          */

/* rebuild spf and extend prefix up to new_max (inclusive) */
static void ensure_capacity(idx_t new_max)
{
    /* Allocate or grow the spf array */
    idx_t *tmp_spf = (idx_t *)realloc(spf, (new_max + 1) * sizeof(idx_t));
    if (!tmp_spf) abort();
    spf = tmp_spf;

    /* Allocate or grow the prefix array */
    seq_t *tmp_prefix = (seq_t *)realloc(prefix, (new_max + 1) * sizeof(seq_t));
    if (!tmp_prefix) abort();
    prefix = tmp_prefix;

    /* Build spf from scratch up to new_max */
    for (idx_t i = 0; i <= new_max; ++i) spf[i] = 0;
    spf[0] = 0;
    if (new_max >= 1) spf[1] = 1;

    for (idx_t i = 2; i <= new_max; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = i;
            if (i > new_max / i) continue;
            for (idx_t j = i * i; j <= new_max; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Make sure a(0) is defined if necessary */
    if (memo_max == 0 && new_max >= 0) {
        prefix[0] = (seq_t)1;
    }

    /* Compute prefix sums for new values */
    for (idx_t r = memo_max + 1; r <= new_max; ++r) {
        seq_t prod = 1;
        idx_t temp = r;
        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p % 4 == 1) {
                prod *= (seq_t)(2 * e + 1);
            }
        }
        prefix[r] = prefix[r - 1] + (seq_t)8 * prod;
    }

    memo_max = new_max;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        if (!prefix) {
            prefix = (seq_t *)malloc(sizeof(seq_t));
            if (!prefix) abort();
            prefix[0] = (seq_t)1;
        }
        if (!spf) {
            spf = (idx_t *)malloc(sizeof(idx_t));
            if (!spf) abort();
            spf[0] = 0;
        }
        memo_max = 0;
        return prefix[0];
    }

    if (n > memo_max) ensure_capacity(n);
    return prefix[n];
}
