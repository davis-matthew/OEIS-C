/*
 * A176826  a(n+1) = Sum_{p=0..n} (a(p)*a(n-p)+k)+l for n>=1, where a(0)=1,
 *          a(1)=1, k=1 and l=-1.
 * https://oeis.org/A176826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1; for n≥1 a(n+1)=∑_{p=0..n} a(p)*a(n-p)+n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (k=1, l=‑1)                                                    */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;      /* buffer for computed terms      */
static size_t buffer_size = 0;   /* allocated slots                */
static size_t computed_upto = 0; /* one past highest computed index */

/* Ensure the internal buffer can hold indices 0..n inclusive */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (buffer_size < needed) {
        size_t new_size = (buffer_size == 0) ? 16 : buffer_size * 2;
        if (new_size < needed) new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();     /* allocation failure */
        cache = tmp;
        buffer_size = new_size;
    }
}

/* Compute a(n) using the recurrence, caching results */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    ensure_capacity(n);

    /* Calculate any terms that are not yet cached */
    for (size_t i = computed_upto; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = 0;
            idx_t lim = (idx_t)(i - 1);          /* n in the recurrence   */
            for (idx_t p = 0; p <= lim; ++p) {
                sum += cache[p] * cache[lim - p];
            }
            sum += (seq_t)lim;   /* add n = i‑1          */
            cache[i] = sum;
        }
    }
    computed_upto = (size_t)n + 1;          /* all indices ≤ n are now ready */
    return cache[(size_t)n];
}
