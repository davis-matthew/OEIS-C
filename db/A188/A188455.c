/*
 * A188455  G.f.: 1 = Sum_{n>=0} a(n)*x^n/(1 + 2^n*x)^(2*n+1).
 * https://oeis.org/A188455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188455
   Formula: a(0) = 1
   a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(2*n-k, k) *
          2^(k*(n-k)) * a(n-k)
   (index starts at 0) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */

static seq_t *a_cache = NULL;     /* array of computed a(n) */
static size_t a_cap = 0;          /* allocated capacity */
static idx_t   a_max  = (idx_t)-1; /* highest index already computed */

/* Return a(n) for any n >= 0.  Results are cached and reused. */
seq_t a(idx_t n) {
    /* First call: initialise cache[0] to 1. */
    if (a_max == (idx_t)-1) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* Return cached value if already known. */
    if (n <= a_max) return a_cache[n];

    /* Ensure the cache array is large enough. */
    size_t needed = n + 1;
    if (needed > a_cap) {
        size_t new_cap = a_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, a_cache, a_cap * sizeof(seq_t));
        memset(new_cache + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        free(a_cache);
        a_cache = new_cache;
        a_cap   = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t bin  = binomial(2 * i - k, k);
            seq_t pow2e = pow2(k * (i - k));          /* 2^(k*(i-k)) */
            seq_t term = bin;
            term *= a_cache[i - k];
            term *= pow2e;
            if (k & 1) pos += term;
            else       neg += term;
        }
        seq_t result = pos;
        if (pos >= neg) result = pos - neg;  /* result always non‑negative */
        else result = 0;                     /* safety – should never happen */
        a_cache[i] = result;
    }
    a_max = n;
    return a_cache[n];
}
