/*
 * A042239  Denominators of continued fraction convergents to sqrt(645).
 * https://oeis.org/A042239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute denominators of convergents to sqrt(645)
   using the recurrence for continued‑fraction convergents.
   The sequence arises from the Pell‑type recurrence:
   q_{-1}=0, q_{-2}=1,
   q_k = a_k * q_{k-1} + q_{k-2},  k ≥ 0,
   where the continued‑fraction coefficients for √645 are
   a0 = 25,
   a1..a10 = {2,1,1,12,10,12,1,1,2,50};
   The period after a0 has length 10.
*/
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms          */
static size_t cache_capacity = 0;    /* allocated space for cache        */
static size_t cache_size = 0;        /* number of terms stored (up to cache_size-1) */

/* coefficients of the continued fraction for √645            */
static const seq_t a_seq[] = {
    25,   /* a0 */
    2, 1, 1, 12, 10, 12, 1, 1, 2, 50 /* a1..a10 (period length 10) */
};
static const size_t period_len = 10; /* number of terms in the periodic part */

static void ensure_capacity(size_t need)
{
    if (cache_capacity >= need) return;
    size_t new_cap = cache_capacity ? cache_capacity : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache_size == 0) {
        ensure_capacity(64);
        cache[0] = (seq_t)1;   /* q0 = 1 */
        cache_size = 1;
    }

    /* Extend cache up to index n */
    while (cache_size <= (size_t)n) {
        size_t k = cache_size;          /* next index to compute   */
        seq_t a_k;
        if (k == 0) {
            a_k = a_seq[0];            /* a0 = 25                */
        } else {
            a_k = a_seq[1 + ((k - 1) % period_len)];
        }
        seq_t q_prev  = cache[k - 1];
        seq_t q_prev2 = (k >= 2) ? cache[k - 2] : (seq_t)0;
        seq_t q_new   = a_k * q_prev + q_prev2;

        if (cache_size >= cache_capacity) {
            ensure_capacity(cache_size + 1);
        }
        cache[cache_size++] = q_new;
    }

    return cache[n];
}
