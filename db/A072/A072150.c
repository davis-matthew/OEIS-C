/*
 * A072150  Coordination sequence for AlPO_4-11 structure with respect to node
 *          (Y) where decagon, hexagon and square meet and is adjacent to node
 *          of type (X).
 * https://oeis.org/A072150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + a(n-9) - a(n-12)  for n >= 13
   Initial values (OEIS):
   a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=10, a(5)=13,
   a(6)=17, a(7)=18, a(8)=20, a(9)=25, a(10)=26, a(11)=28, a(12)=33
   The recurrence holds for n >= 13. */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t base_vals[13] = {
    (seq_t)1,  (seq_t)3,  (seq_t)5,  (seq_t)8,  (seq_t)10,
    (seq_t)13, (seq_t)17, (seq_t)18, (seq_t)20, (seq_t)25,
    (seq_t)26, (seq_t)28, (seq_t)33
};

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = (n >= 13) ? n + 1 : 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 13; ++i) cache[i] = base_vals[i];
        for (idx_t i = 13; i < cache_size; ++i) {
            cache[i] = cache[i - 3] + cache[i - 9] - cache[i - 12];
        }
    } else if (n >= cache_size) {
        idx_t old = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        for (idx_t i = old; i < cache_size; ++i) {
            cache[i] = cache[i - 3] + cache[i - 9] - cache[i - 12];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
