/*
 * A145543  Denominators in continued fraction expansion of sqrt(3/5).
 * https://oeis.org/A145543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = denominators of the continued fraction of sqrt(3/5).
 *            Recurrence: a(1)=1, a(2)=4, for n≥3 a(n) = ((n%2==0)?3:2) *
 *            a(n-1) + a(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;        /* cache[n-1] = a(n)  */
static size_t cached = 0;          /* number of cached terms */

static void ensure_cache(idx_t n)
{
    if ((size_t)n > cached) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;

        for (idx_t i = (idx_t)cached + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 1;
            } else if (i == 2) {
                val = 4;
            } else {
                seq_t mult = (i % 2 == 0) ? (seq_t)3 : (seq_t)2;
                val = mult * cache[i - 2] + cache[i - 3];
            }
            cache[i - 1] = val;
        }
        cached = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence defined for n ≥ 1 */
    ensure_cache(n);
    return cache[n - 1];
}
