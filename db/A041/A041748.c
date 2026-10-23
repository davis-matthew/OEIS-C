/*
 * A041748  Numerators of continued fraction convergents to sqrt(394).
 * https://oeis.org/A041748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A041748: Numerators of continued‑fraction convergents to sqrt(394).
 * Recurrence (from the CF expansion of sqrt(394)):
 *   let a0 = floor(sqrt(394)) = 19
 *   m0 = 0, d0 = 1
 *   for k ≥ 1:
 *     ak  = floor((a0 + mk) / dk)
 *     mk+1 = dk * ak – mk
 *     dk+1 = (394 – (mk+1)^2) / dk
 *     pk   = ak * pk-1 + pk-2      (p-1 = 1, p0 = a0)
 *
 * This implementation builds the numerator sequence on demand, caching
 * all intermediates.  It works for any unsigned integer type `seq_t`
 * (including arbitrary‑precision classes) and for idx_t = uint64_t.
 */

#include <stdint.h>
#include <stdlib.h>   /* malloc, realloc, abort */

static idx_t cache_cap = 0;          /* capacity of the cached arrays     */
static idx_t cache_len = 0;          /* number of values stored           */
static seq_t *cache_p = NULL;        /* p[n]   (numerators)               */
static idx_t *cache_m = NULL;        /* m[n]   (partial numerators)       */
static idx_t *cache_d = NULL;        /* d[n]   (partial denominators)     */
static idx_t *cache_a = NULL;        /* a[n]   (partial quotients)        */
static const idx_t D = 394;          /* constant inside the CF recurrence */

/* Allocate or double the cached arrays to at least 'need' elements. */
static void ensure_cache(idx_t need)
{
    if (cache_cap >= need + 1)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need + 1)
        new_cap = need + 1;

    seq_t *new_p   = (seq_t *)realloc(cache_p, new_cap * sizeof(seq_t));
    idx_t *new_m   = (idx_t *)realloc(cache_m, new_cap * sizeof(idx_t));
    idx_t *new_d   = (idx_t *)realloc(cache_d, new_cap * sizeof(idx_t));
    idx_t *new_a   = (idx_t *)realloc(cache_a, new_cap * sizeof(idx_t));

    if (!new_p || !new_m || !new_d || !new_a)
        abort();   /* allocation failure – fail fast */

    cache_p = new_p;
    cache_m = new_m;
    cache_d = new_d;
    cache_a = new_a;
    cache_cap = new_cap;
}

/* Initialise the first element (n = 0).  Must be called before any use. */
static void init_first(void)
{
    /* Ensure storage for at least one element. */
    ensure_cache(0);

    const idx_t a0 = 19;            /* floor(sqrt(394)) */
    cache_p[0] = (seq_t)a0;         /* p0  = a0 */
    cache_m[0] = 0;                 /* m0  = 0  */
    cache_d[0] = 1;                 /* d0  = 1  */
    cache_a[0] = a0;                /* a0  = 19 */
    cache_len = 1;
}

/* Extend the cache so that indices 0…n are computed. */
static void extend_to(idx_t n)
{
    if (cache_len == 0)
        init_first();

    if (n < cache_len)
        return;   /* already have it */

    /* Prepare to compute from the last known index. */
    idx_t last = cache_len - 1;
    idx_t m_prev = cache_m[last];
    idx_t d_prev = cache_d[last];
    idx_t a_prev = cache_a[last];
    seq_t p_prev  = cache_p[last];
    seq_t p_prev2 = (last == 0) ? (seq_t)1 : cache_p[last - 1];

    /* Grow storage to include the new indices. */
    ensure_cache(n);

    for (idx_t k = last + 1; k <= n; ++k) {
        /* Compute next partial quotient and the convergents. */
        idx_t m_next = d_prev * a_prev - m_prev;
        idx_t d_next = (D - m_next * m_next) / d_prev;
        idx_t a_next = (19 + m_next) / d_next;

        seq_t p_next = a_next * p_prev + p_prev2;

        /* Store the computed values. */
        cache_p[k] = p_next;
        cache_m[k] = m_next;
        cache_d[k] = d_next;
        cache_a[k] = a_next;

        /* Shift registers for next iteration. */
        m_prev = m_next;
        d_prev = d_next;
        a_prev = a_next;
        p_prev2 = p_prev;
        p_prev = p_next;
    }

    cache_len = n + 1;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    extend_to(n);
    return cache_p[n];
}
