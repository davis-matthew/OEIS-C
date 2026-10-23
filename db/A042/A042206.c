/*
 * A042206  Numerators of continued fraction convergents to sqrt(629).
 * https://oeis.org/A042206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(629) (continued
 *            fraction of sqrt(629) has partial quotients 25,12,1,1,12,50).
 *            Recurrence: a(0)=25, a(1)=301, a(2)=326, a(3)=627, a(4)=7850,
 *            a(5)=393127, a(6)=4725374, a(7)=5118501, a(8)=9843875,
 *            a(9)=123245001; for n≥10, a(n)=15700*a(n‑5)+a(n‑10)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached a(i) values */
static size_t cache_size = 0;    /* number of computed terms (cache_size == last_computed + 1) */
static size_t cache_cap   = 0;    /* allocated capacity */

/* State of the continued‑fraction generator (kept across calls) */
static idx_t  cf_m   = 0;        /* m_k */
static idx_t  cf_d   = 1;        /* d_k */
static idx_t  cf_a   = 25;       /* a_k (partial quotient) */
static seq_t  prev1 = 1;         /* p_{k-1}  (previous numerator) */
static seq_t  prev2 = 0;         /* p_{k-2}  */
static int    initialized = 0;   /* true when the generator has been set up */

/* Ensure cache contains a(0) … a(n) */
static void
ensure_cache(idx_t n)
{
    if (!initialized) {
        /* Initialise the first term a(0) */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* compute a(0) */
        prev1 = 1;
        prev2 = 0;
        cache[0] = (seq_t)cf_a * prev1 + prev2;   /* 25 */
        prev1 = cache[0];
        prev2 = 1;
        cache_size = 1;
        initialized = 1;
    }

    /* Grow cache if necessary */
    while (cache_size <= (size_t)n) {
        if (cache_size >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            cache = tmp;
        }

        /* Compute next term a_cache_size */
        /* first generate the next partial quotient cf_a for index = cache_size */
        if (cache_size == 1) {
            /* we already have cf_a = a_0 = 25, the state after index 0
               is ready to produce a_1 */
        }
        /* use current cf_m, cf_d, cf_a (which is a_{k}) to produce a_{k+1} */
        idx_t m_next = cf_d * cf_a - cf_m;
        idx_t d_next = (629 - (m_next * m_next)) / cf_d;
        idx_t a_next = (25 + m_next) / d_next;

        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;

        /* compute p_{k+1} = a_{k+1} * prev1 + prev2 */
        seq_t p_next = (seq_t)cf_a * prev1 + prev2;

        /* store in cache */
        cache[cache_size] = p_next;

        /* update previous values for next iteration */
        prev2 = prev1;
        prev1 = p_next;

        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
