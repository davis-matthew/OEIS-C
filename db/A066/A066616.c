/*
 * A066616  a(1) = 1; a(n) = n*a(n-1) if n does not divide a(n-1), otherwise
 *          a(n) = a(n-1).
 * https://oeis.org/A066616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n-1) if n | a(n-1) a(n)=n*a(n-1) otherwise (n>=2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] unused, cache[1] holds a(1)  */
static idx_t cache_size = 0;       /* allocated slots (including index 0) */
static idx_t computed_max = 0;     /* largest n computed so far */

/* Ensure cache can address up to index n (inclusive)  */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);                 /* out of memory */
    cache = new_cache;
    cache_size = n + 1;
}

/* Compute a(n).  The function is memoised so that repeated calls
   or out‑of‑order calls are fast. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for 0, but safe */
    ensure_cache(n);

    /* compute values up to n if not yet done */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t prev = cache[k - 1];
            seq_t kk   = (seq_t)k;
            if (prev % kk == 0) {
                cache[k] = prev;
            } else {
                cache[k] = prev * kk;
            }
        }
    }
    computed_max = n > computed_max ? n : computed_max;
    return cache[n];
}
