/*
 * A040517  Continued fraction for sqrt(541).
 * https://oeis.org/A040517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continue fraction terms of sqrt(541)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   allocate_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(541)) = 23, subsequent a(n) given by the standard CF algorithm. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;           /* cache of computed terms */
static size_t cache_cap = 0;          /* allocated capacity of cache */
static size_t cache_len = 0;          /* number of terms currently cached */

static idx_t cur_m = 0;               /* current m in the CF algorithm */
static idx_t cur_d = 1;               /* current d in the CF algorithm */
static idx_t cur_a = 0;               /* current a (next term to output) */
static bool initialized = false;     /* flag indicating whether initialization has occurred */

static void init(void)
{
    if (initialized) return;
    const idx_t N = 541;
    const idx_t a0 = (idx_t)((seq_t)0 + (seq_t)0);   /* placeholder for type consistency */
    /* Compute integer part of sqrt(541) */
    const idx_t sqrtN = 23;             /* floor(sqrt(541)) */
    cur_m = 0;
    cur_d = 1;
    cur_a = sqrtN;                     /* a0 */

    /* initial cache capacity */
    cache_cap = 16;
    cache = allocate_seq_array(cache_cap);
    cache_len = 0;                     /* nothing cached yet */

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    /* Extend cache up to the requested index. */
    while (cache_len <= n) {
        if (cache_len == 0) {
            /* a0 is already in cur_a */
        } else {
            /* compute next CF term */
            idx_t m_next = cur_d * cur_a - cur_m;
            idx_t d_next = (541 - m_next * m_next) / cur_d;
            idx_t a_next = (23 + m_next) / d_next;
            cur_m = m_next;
            cur_d = d_next;
            cur_a = a_next;
        }

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)cur_a;
    }

    return cache[n];
}
