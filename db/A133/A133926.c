/*
 * A133926  Number of equivalence classes of compositions of n into parts of
 *          size 2 and 3 under the following equivalence relation: We make a
 *          "move" by changing three consecutive 2s into two consecutive 3s or
 *          vice versa. Two compositions are equivalent if we can reach one
 *          from the other by a series of moves.
 * https://oeis.org/A133926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 92, u32 n <= 187, u64 n <= 377,
 *            u128 n <= 757, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-3) - a(n-6)
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=2 */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;
static idx_t a_cache_used = 0;

static const seq_t init_vals[6] = {1, 0, 1, 1, 1, 2};

static void a_ensure_capacity(idx_t needed)
{
    if (a_cache_cap <= needed) {
        idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 6;
        while (new_cap <= needed) new_cap *= 2;
        seq_t *new_cache = alloc_seq_array((size_t)new_cap);
        if (a_cache) {
            for (idx_t i = 0; i < a_cache_used; ++i) {
                new_cache[i] = a_cache[i];
            }
            seq_free_array(a_cache);
        }
        a_cache = new_cache;
        a_cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (a_cache == NULL) {
        a_cache_cap = 6;
        a_cache = alloc_seq_array((size_t)a_cache_cap);
        for (idx_t i = 0; i < 6; ++i) {
            a_cache[i] = init_vals[i];
        }
        a_cache_used = 6;
    }

    a_ensure_capacity(n);

    while (a_cache_used <= n) {
        idx_t i = a_cache_used;
        a_cache[i] = a_cache[i - 2] + a_cache[i - 3] - a_cache[i - 6];
        ++a_cache_used;
    }

    return a_cache[n];
}
