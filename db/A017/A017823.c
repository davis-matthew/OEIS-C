/*
 * A017823  Expansion of 1/(1-x^3-x^4-x^5-x^6-x^7-x^8-x^9-x^10).
 * https://oeis.org/A017823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 63, u64 n <= 123,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10),
   initial values: a(0)=1, a(1)=0, a(2)=0 */

/* Standard headers (required for realloc, SIZE_MAX, size_t). */
#include <stddef.h>
#include <stdlib.h>

/* --------------  Static cache  --------------------- */
static seq_t *cache          = NULL;    /* cache[0..computed_max] */
static size_t cache_cap      = 0;       /* allocated space (number of entries) */
static size_t computed_max   = SIZE_MAX;/* highest index already stored;
                                          SIZE_MAX means "none yet" */

/* --------------------------------------------------- */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    seq_t *newp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache       = newp;
    cache_cap   = needed;
}

static void
init_base(void)
{
    /* make sure we have entries 0..2 available */
    ensure_capacity(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    computed_max = 2;
}

/* --------------------------------------------------- */
/* Compute a(0)…a(n) and store them in the cache.      */
static void
compute_up_to(idx_t n)
{
    size_t tn = (size_t)n;

    /* first call: initialise base values */
    if (computed_max == SIZE_MAX) init_base();

    if (tn <= computed_max) return;                 /* nothing to do */

    ensure_capacity(tn + 1);                         /* space for 0..n */

    for (size_t i = computed_max + 1; i <= tn; ++i) {
        seq_t sum = 0;
        for (int k = 3; k <= 10; ++k) {
            if (i >= (size_t)k) sum += cache[i - k];
        }
        cache[i] = sum;
    }
    computed_max = tn;
}

/* --------------------------------------------------- */
/* Public function requested by the test harness.      */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
