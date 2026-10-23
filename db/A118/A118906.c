/*
 * A118906  a(0) = 1; a(n) = |a(n-1) - n| if n is not one of the earlier terms
 *          of the sequence, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A118906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A118906:
   a(0) = 1
   for n ≥ 1:
     if n is not one of the earlier terms a(0)..a(n-1) then
        a(n) = |a(n-1) - n|
     else
        a(n) = a(n-1) + n
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache   = NULL;  /* stored sequence values          */
static idx_t  size_c  = 0;     /* how many values are computed    */
static size_t cap_c   = 0;     /* allocated capacity               */

/* Grow cache to be able to store up to new_cap elements. */
static void
ensure_cache_capacity(idx_t new_cap)
{
    if (new_cap <= (idx_t)cap_c)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed – unexpected   */
    cache = tmp;
    cap_c = new_cap;
}

seq_t
a(idx_t n)
{
    /* Return already computed value if available. */
    if (n < size_c)
        return cache[(size_t)n];

    /* Make sure the cache can hold everything up to index n. */
    ensure_cache_capacity(n + 1);

    /* Initialise a(0) if the cache is empty. */
    if (size_c == 0) {
        cache[0] = (seq_t)1;
        size_c  = 1;
        if (n == 0)
            return cache[0];
    }

    /* Compute values from the next unused index up to n. */
    for (idx_t k = size_c; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;   /* convert index to seq_t for comparison */
        bool present = false;

        /* Check whether k appears among a(0)..a(k-1). */
        for (idx_t i = 0; i < k; ++i) {
            if (cache[(size_t)i] == k_seq) {
                present = true;
                break;
            }
        }

        if (present) {
            /* a(k) = a(k-1) + k */
            cache[(size_t)k] = cache[(size_t)(k - 1)] + k_seq;
        } else {
            /* a(k) = |a(k-1) - k| */
            seq_t prev = cache[(size_t)(k - 1)];
            seq_t diff = (prev >= k_seq) ? (prev - k_seq) : (k_seq - prev);
            cache[(size_t)k] = diff;
        }
    }

    size_c = n + 1;
    return cache[(size_t)n];
}
