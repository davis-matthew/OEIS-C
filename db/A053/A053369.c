/*
 * A053369  Linear recursion with Catalan numbers.
 * https://oeis.org/A053369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*n + 2)*Catalan(n) (Catalan(n) = (2n)!/(n!(n+1)!) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cat_cache = NULL;   /* cache of Catalan numbers C(0)..C(k) */
static idx_t cat_cached = 0;      /* highest n cached so far (index) */

/* Ensure cat_cache has space for indices 0..need */
static void ensure_cat_cache(idx_t need)
{
    if (cat_cached >= need) return;
    idx_t new_size = (cat_cached == 0) ? 1 : cat_cached << 1;
    while (new_size <= need) new_size <<= 1;
    cat_cache = (seq_t*)realloc(cat_cache, new_size * sizeof(seq_t));
    /* Initialize the new entries up to need */
    if (cat_cached == 0) {
        cat_cache[0] = 1;
    }
    for (idx_t i = cat_cached; i < new_size; i++) {
        /* Use recurrence: C(i+1) = C(i) * (4*i+2) / (i+2) */
        if (i == 0) continue; /* C(0) already set */
        seq_t num = cat_cache[i-1] * ( (seq_t)(4*(i-1)+2) );
        cat_cache[i]   = num / ( (seq_t)(i+1) ); /* exact division */
    }
    cat_cached = new_size - 1;
}

static void compute_catalan_upto(idx_t n)
{
    if (cat_cached >= n) return;
    ensure_cat_cache(n);
    /* All values already computed during ensure_cat_cache */
}

/* Return the nth Catalan number (C(0)=1) */
static seq_t catalan(idx_t n)
{
    if (cat_cached == 0) {
        cat_cache = (seq_t*)malloc(sizeof(seq_t));
        cat_cache[0] = 1;
        cat_cached = 0;
    }
    if (cat_cached < n) {
        /* Need to extend cache */
        ensure_cat_cache(n);
    }
    return cat_cache[n];
}

seq_t a(idx_t n)
{
    /* Compute Catalan(n) */
    seq_t cat_n = catalan(n);
    /* Compute (7*n + 2) * cat_n */
    seq_t mul = ((seq_t)n * 7) + 2;
    return mul * cat_n;
}
