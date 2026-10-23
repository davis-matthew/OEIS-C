/*
 * A143608  A005319 and A002315 interleaved.
 * https://oeis.org/A143608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=4, a(3)=7; for n>=4: a(n) = 6*a(n-2) -
 *            a(n-4) (OEIS A143608)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;   // pointer to cached values
static size_t a_cache_size = 0; // number of values currently stored
static size_t a_cache_cap  = 0; // capacity of the cache array

static void ensure_cache(idx_t n)
{
    if ((size_t)n < a_cache_size) return;        // already computed

    // reserve space up to n
    size_t need = (size_t)n + 1;
    if (need > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    // base values
    if (a_cache_size == 0) a_cache[0] = (seq_t)0;
    if (a_cache_size <= 1) a_cache[1] = (seq_t)1;
    if (a_cache_size <= 2) a_cache[2] = (seq_t)4;
    if (a_cache_size <= 3) a_cache[3] = (seq_t)7;

    // compute missing terms
    for (size_t i = a_cache_size; i <= (size_t)n; ++i) {
        if (i == 0)
            a_cache[i] = (seq_t)0;        // already set above
        else if (i == 1)
            a_cache[i] = (seq_t)1;
        else if (i == 2)
            a_cache[i] = (seq_t)4;
        else if (i == 3)
            a_cache[i] = (seq_t)7;
        else {
            /* recurrence: a(i) = 6 * a(i-2) - a(i-4) */
            a_cache[i] = (seq_t)((seq_t)6 * a_cache[i - 2] - a_cache[i - 4]);
        }
    }
    a_cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
