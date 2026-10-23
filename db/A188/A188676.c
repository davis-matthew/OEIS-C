/*
 * A188676  Alternate partial sums of the binomial coefficients
 *          binomial(3*n,n).
 * https://oeis.org/A188676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} (-1)^{n-k} * binomial(3*k,k) Recurrence:
 *            a(0)=1, a(n)=binomial(3*n,n)-a(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are assumed to be defined externally. */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of a(n) and B(n)=binomial(3*n,n). */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t  cache_cap = 0;   /* Allocated capacity for both arrays. */
static idx_t  cached_until = (idx_t)-1;  /* Largest n computed (unsigned max means none). */

/* Ensure that a_cache and b_cache can hold at least 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap ? cache_cap << 1 : 8);
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp_a) abort();        /* allocation failed */
    seq_t *tmp_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    if (!tmp_b) {
        free(tmp_a); abort();
    }
    a_cache = tmp_a;
    b_cache = tmp_b;
    cache_cap = new_cap;
}

/* Compute binomial(3*n, n) from binomial(3*(n-1), n-1). */
static seq_t next_binom(seq_t prev, idx_t n)
{
    /* prev = B_{n-1}. Return B_n. */
    if (sizeof(seq_t) <= 8) {
        /* 64‑bit or smaller: use 128‑bit intermediate to avoid overflow. */
        unsigned __int128 t = (unsigned __int128)prev;
        t *= (unsigned __int128)(3*n);
        t *= (unsigned __int128)(3*n - 1);
        t *= (unsigned __int128)(3*n - 2);
        t /= (unsigned __int128)n;
        t /= (unsigned __int128)(2*n);
        t /= (unsigned __int128)(2*n - 1);
        return (seq_t)t;
    } else {
        /* Arbitrary‑precision or >64‑bit: rely on built‑in operator overloads. */
        seq_t t = prev;
        t *= (seq_t)(3*n);
        t *= (seq_t)(3*n - 1);
        t *= (seq_t)(3*n - 2);
        t /= (seq_t)n;
        t /= (seq_t)(2*n);
        t /= (seq_t)(2*n - 1);
        return t;
    }
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cached_until == (idx_t)-1) {
        ensure_cache_capacity(16);
        a_cache[0] = (seq_t)1;      /* a(0)=1 */
        b_cache[0] = (seq_t)1;      /* binom(0,0)=1 */
        cached_until = 0;
    }

    if (n <= cached_until)
        return a_cache[n];

    /* Compute missing values up to n. */
    for (idx_t k = cached_until + 1; k <= n; ++k) {
        seq_t B_new = next_binom(b_cache[k-1], k);
        seq_t a_new = B_new - a_cache[k-1];
        ensure_cache_capacity(k + 1);
        b_cache[k] = B_new;
        a_cache[k] = a_new;
    }
    cached_until = n;
    return a_cache[n];
}
