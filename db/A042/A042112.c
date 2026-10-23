/*
 * A042112  Numerators of continued fraction convergents to sqrt(581).
 * https://oeis.org/A042112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042112: numerators of the convergents to sqrt(581).
   Recurrence for n > 47:
         a(n) = 304142307950 * a(n-24) - a(n-48)
   The first few terms (n = 0 … 6) are:
         a(0)=24, a(1)=217, a(2)=241, a(3)=458,
         a(4)=699, a(5)=1157, a(6)=1856, …
   All other terms are obtained from the continued‑fraction algorithm
   for sqrt(581) up to a(47) and then from the recurrence above. */
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static idx_t next_index = 0;          /* next index that must be computed */

/* Parameters of the continued fraction for sqrt(581). */
static idx_t cf_m   = 0;   /* m_k   */
static idx_t cf_d   = 1;   /* d_k   */
static idx_t cf_a   = 24;  /* a_k   (current a_k) */

/* Ensure that the cache is big enough for indices up to n. */
static void ensure_cache_size(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                        /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < next_index)
        return cache[n];

    ensure_cache_size(n);

    const seq_t C34 = (seq_t)304142307950ULL;  /* recurrence constant */

    for (idx_t k = next_index; k <= n; ++k)
    {
        if (k <= 47)                       /* continued fraction part */
        {
            if (k == 0)
                cache[0] = (seq_t)cf_a;   /* p0 = a0 */
            else
            {
                seq_t p_prev     = cache[k - 1];
                seq_t p_prevPrev = (k == 1 ? (seq_t)1 : cache[k - 2]);
                cache[k] = (seq_t)cf_a * p_prev + p_prevPrev;
            }

            /* Compute a_{k+1} for the next iteration. */
            if (k < 47)
            {
                idx_t m_next = cf_d * cf_a - cf_m;
                idx_t d_next = (581ULL - m_next * m_next) / cf_d;
                idx_t a_next = (24 + m_next) / d_next;   /* a0 = 24 for sqrt(581) */

                cf_m = m_next;
                cf_d = d_next;
                cf_a = a_next;
            }
        }
        else                                /* recurrence for n > 47 */
        {
            seq_t term24 = cache[k - 24];
            seq_t term48 = cache[k - 48];
            cache[k] = C34 * term24 - term48;
        }
    }

    next_index = n + 1;
    return cache[n];
}
