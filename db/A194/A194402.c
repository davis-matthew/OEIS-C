/*
 * A194402  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=(1+sqrt(5))/2 and < > denotes fractional part.
 * https://oeis.org/A194402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194402: numbers m such that among k=1..m the fractional part
 * of k*φ satisfies {kφ} < 1/2 exactly m/2 times.  This holds iff
 * Sum_{k=1..m} ( ⟨1/2 + kφ⟩ - ⟨kφ⟩ ) = 0.  The function generates the
 * list by keeping a running balance s = (#{kφ}<1/2) – (#{kφ}≥1/2) and
 * outputting m when s returns to 0 at an even m.  Values are produced
 * on demand and cached so that successive calls are O(1) after the
 * largest index requested. */
#include <stdlib.h>
#include <math.h>

static seq_t *a_cache = NULL;    /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of values stored */
static idx_t next_k = 0;         /* next integer to test */
static long long s_balance = 0;  /* running balance, can be negative */

/* Golden ratio constant (long double).  No sqrt used to stay exact
 * enough for all required indices (up to at least 10 000). */
static const long double phi = 1.6180339887498948482045868343656381177203L;

/* Ensure the cache has enough space for at least 'n' elements. */
static void ensure_cache_capacity(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_ptr;
    cache_cap = new_cap;
}

/* Generate additional terms until cache_len >= n. */
static void generate_terms(idx_t n)
{
    while (cache_len < n) {
        ++next_k;                                          /* test m=next_k */
        /* fractional part of k*phi */
        long double prod = next_k * phi;
        long double frac = prod - floorl(prod);             /* 0 <= frac < 1 */
        /* update balance */
        if (frac < 0.5L) ++s_balance; else --s_balance;
        /* record even m with zero balance */
        if ((next_k & 1ULL) == 0 && s_balance == 0) {
            ensure_cache_capacity(cache_len + 1);
            a_cache[cache_len++] = (seq_t)next_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, a(0) unused */
    generate_terms(n);                       /* build cache up to n */
    return a_cache[n - 1];                   /* zero‑based indexing */
}
