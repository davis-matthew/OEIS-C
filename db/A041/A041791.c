/*
 * A041791  Denominators of continued fraction convergents to sqrt(416).
 * https://oeis.org/A041791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A041791 – Denominators of continued fraction convergents to √416
   Formula: a(n) = 10402*a(n‑8) – a(n‑16)  for n > 15
   Initial values (n = 0 … 15):
   1, 2, 3, 5, 48, 53, 101, 255,
   10301, 20857, 31158, 52015, 499293, 551308, 1050601, 2652510
*/

#include <stdlib.h>

static seq_t *cache = NULL;    /* cache of computed terms */
static idx_t cache_size = 0;   /* number of terms stored (max index + 1) */

#if defined(__cplusplus)
    /* cast for C++ compiler */
    #define REALLOC(ptr, newsz) reinterpret_cast<seq_t*>(realloc(ptr, newsz))
#else
    /* cast for C compiler */
    #define REALLOC(ptr, newsz) ((seq_t*)realloc(ptr, newsz))
#endif

/* ensure that terms up to and including index n are computed */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)                 /* already available          */
        return;

    idx_t new_size = n + 1;             /* needed size (indices 0..n) */
    seq_t *tmp = REALLOC(cache, new_size * sizeof(seq_t));
    if (!tmp) { abort(); }
    cache = tmp;

    /* constant initial values for n = 0 … 15 */
    static const seq_t init_vals[16] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
        (seq_t)48, (seq_t)53, (seq_t)101, (seq_t)255,
        (seq_t)10301, (seq_t)20857, (seq_t)31158,
        (seq_t)52015, (seq_t)499293, (seq_t)551308,
        (seq_t)1050601, (seq_t)2652510
    };

    /* fill new entries */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i < 16)
            cache[i] = init_vals[i];
        else
            cache[i] = (seq_t)10402 * cache[i - 8] - cache[i - 16];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
