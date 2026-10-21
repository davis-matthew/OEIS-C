/*
 * A094620  Expansion of x*(11 + 22*x + 20*x^2)/((1-x)*(1+x)*(1 - 10*x^2)).
 * https://oeis.org/A094620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A094620 – Expansion of x*(11 + 22*x + 20*x^2)/((1-x)*(1+x)*(1 - 10*x^2))
   Recurrence (for n>3):
     a(n) = 11*a(n-2) - 10*a(n-4)
   Initial values:
     a(0)=0,  a(1)=11,  a(2)=22,  a(3)=141
   The implementation caches computed terms so that calls are O(1) after the
   first precomputation.  All arithmetic is performed in seq_t.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cached = NULL;     /* array of computed terms */
static size_t cached_size = 0;   /* number of terms currently computed   */
static size_t cached_cap  = 0;   /* capacity of the allocated array      */

static void ensure_cache_cap(size_t n) /* ensure capacity for index n */
{
    if (n < cached_cap) return;
    size_t new_cap = cached_cap ? cached_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cached, new_cap * sizeof(seq_t));
    cached = new_cache;
    cached_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < cached_size) return cached[idx];

    ensure_cache_cap(idx);

    for (size_t i = cached_size; i <= idx; ++i)
    {
        if (i == 0)           cached[i] = (seq_t)0;
        else if (i == 1)      cached[i] = (seq_t)11;
        else if (i == 2)      cached[i] = (seq_t)22;
        else if (i == 3)      cached[i] = (seq_t)141;
        else /* i >= 4 */     cached[i] = (seq_t)11 * cached[i-2] - (seq_t)10 * cached[i-4];
    }
    cached_size = idx + 1;
    return cached[idx];
}
