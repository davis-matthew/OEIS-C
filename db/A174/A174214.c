/*
 * A174214  a(n) = a(n-1)+1, if the previous term a(n-1) and n-1-(-1)^n are
 *          coprime, else a(n)=2*n-4.
 * https://oeis.org/A174214
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1 if gcd(a(n-1), n-1-(-1)^n) = 1 else a(n) = 2*n-4
 *            Offset: 9 (a(9) = 14)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static idx_t  cache_len = 0;      /* current allocated array size (indexable up to cache_len-1) */
static idx_t  last_computed = 0;  /* highest index for which cache[i] is set */
static const idx_t OFFSET = 9;

/* helper to resize cache if needed */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_len) return;
    idx_t new_len = needed + 1;          /* allocate indices 0..needed inclusive */
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) abort();             /* die on OOM – test harness will not hit this */
    /* copy existing values */
    for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

/* compute a(n) for n >= 9 */
seq_t a(idx_t n)
{
    if (n < OFFSET) return (seq_t)0;     /* behaviour below offset is irrelevant */

    ensure_cache(n);
    if (last_computed < OFFSET) {
        /* initialise the very first term */
        cache[OFFSET] = (seq_t)14;
        last_computed = OFFSET;
    }
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == OFFSET) {          /* should not happen normally, but keep safety */
            cache[i] = (seq_t)14;
            continue;
        }
        seq_t prev = cache[i - 1];
        idx_t m  = (i % 2 == 0) ? i - 2 : i;          /* n-1-(-1)^n */
        seq_t m_seq = (seq_t)m;
        seq_t g = gcd(prev, m_seq);
        if (g == (seq_t)1) {
            cache[i] = prev + 1;
        } else {
            cache[i] = ((seq_t)2 * (seq_t)i) - 4;
        }
    }
    last_computed = n;
    return cache[n];
}
