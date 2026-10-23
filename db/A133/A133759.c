/*
 * A133759  Numbers that are the sum of a nonzero pentagonal number and a
 *          nonzero square in at least one way.
 * https://oeis.org/A133759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   can_represent, compute_pentagonal, contains_pentagonal,
 *            ensure_pentagonals_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n‑th integer that can be written as P(i)+j²,
   where P(i)=i(3i‑1)/2 is a pentagonal number and i>0, j>0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/can_represent.h"
#include "../utils/compute_pentagonal.h"
#include "../utils/contains_pentagonal.h"
#include "../utils/ensure_pentagonals_up_to.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;

    /* Pentagonal numbers storage */
    static seq_t *pent = NULL;
    static size_t pent_sz = 0, pent_cap = 0;
    static idx_t next_pent_k = 1;

    while (cache_sz < n) {
        seq_t m;
        if (cache_sz == 0)
            m = 2;                    /* first term */
        else
            m = cache[cache_sz - 1] + 1;

        while (!can_represent(m,
                              &pent, &pent_sz, &pent_cap, &next_pent_k))
            ++m;

        if (cache_sz == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = m;
    }
    return cache[n - 1];
}
