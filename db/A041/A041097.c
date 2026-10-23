/*
 * A041097  Denominators of continued fraction convergents to sqrt(56).
 * https://oeis.org/A041097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 30*a(n-2) - a(n-4); a(0)=1, a(1)=2, a(2)=29, a(3)=60 */
#include <stdlib.h>

/* Cache of computed terms */
static seq_t *cached = NULL;
static size_t cached_cap = 0;

/* Ensure that cached[0..n] are computed */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cached_cap) return;          /* already computed */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cached = (seq_t*)realloc(cached, new_cap * sizeof(seq_t));
    if (!new_cached) abort();                    /* allocation failure */
    cached = new_cached;
    /* fill new entries */
    for (idx_t i = (idx_t)cached_cap; i <= n; ++i)
    {
        if (i == 0)            cached[i] = 1;
        else if (i == 1)       cached[i] = 2;
        else if (i == 2)       cached[i] = 29;
        else if (i == 3)       cached[i] = 60;
        else
        {
            seq_t t = cached[i - 2];
            t *= 30;
            t -= cached[i - 4];
            cached[i] = t;
        }
    }
    cached_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cached[(size_t)n];
}
