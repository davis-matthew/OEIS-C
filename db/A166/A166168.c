/*
 * A166168  G.f.: exp( Sum_{n>=1} Lucas(n^2)*x^n/n ) where Lucas(n) =
 *          A000204(n).
 * https://oeis.org/A166168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; * a(n) = (1/n) * sum_{k=1..n} Lucas(k^2) * a(n-k),
 *            n>=1. * Lucas(n) = Fib(n-1) + Fib(n+1), Fib fast‐doubling is
 *            used
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#else
#include <vector>
#endif

/* ---------- Lucas + Fibonacci helpers ---------- */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t f, f_next;
    fib_pair(n >> 1, &f, &f_next);
    seq_t c = f * (((seq_t)2 * f_next) - f);
    seq_t d = f * f + f_next * f_next;
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

static seq_t lucas_at(idx_t m)
{
    seq_t f, f_next;
    fib_pair(m, &f, &f_next);        /* F_m , F_{m+1} */
    return (seq_t)2 * f_next - f;    /* 2*F_{m+1} - F_m = L_m */
}

/* ---------- C++‑specific storage ---------- */
#ifdef __cplusplus
typedef std::vector<seq_t> seq_vec;
static seq_vec a_cache;      /* a(0..current) */
static seq_vec lucas_cache;  /* Lucas(k^2) for k=0..current */
#else
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static idx_t  a_cache_done = 0;
static seq_t *lucas_cache  = NULL;
static size_t lucas_cache_cap  = 0;
static idx_t  lucas_cache_done = 0;
#endif

/* ---------- Cache helpers ---------- */
#ifndef __cplusplus
static void ensure_cache(idx_t n)
{
    /* Grow a_cache to hold indices 0..n */
    if (a_cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Grow lucas_cache to hold indices 0..n (k from 1..n) */
    if (lucas_cache_done < n) {
        if (lucas_cache_cap < (size_t)(n + 1)) {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
            lucas_cache = tmp;
            lucas_cache_cap = new_cap;
        }
        for (idx_t k = lucas_cache_done + 1; k <= n; ++k) {
            lucas_cache[k] = lucas_at(k * k);
        }
        lucas_cache_done = n;
    }

    /* Compute a(i) for all missing indices */
    if (a_cache_done == 0) {
        a_cache[0] = 1;
        a_cache_done = 1;
    }
    for (idx_t i = a_cache_done; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += lucas_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;
    }
    a_cache_done = n + 1;
}
#else
static void ensure_cache(idx_t n)
{
    /* ---- Lucas cache ---- */
    size_t old_luc = lucas_cache.size();
    if (old_luc <= (size_t)n) {
        lucas_cache.resize(n + 1);
        for (idx_t k = old_luc; k <= n; ++k) {
            lucas_cache[k] = lucas_at(k * k);
        }
    }

    /* ---- a_cache ---- */
    size_t old_a = a_cache.size();
    if (old_a == 0) {
        a_cache.resize(1);
        a_cache[0] = 1;
        old_a = 1;
    }
    if (old_a <= (size_t)n) {
        a_cache.resize(n + 1);
        for (idx_t i = old_a; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += lucas_cache[k] * a_cache[i - k];
            }
            a_cache[i] = sum / (seq_t)i;
        }
    }
}
#endif

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
#ifdef __cplusplus
    return a_cache[(size_t)n];
#else
    return a_cache[n];
#endif
}
