/*
 * A229393  Number of shapes of balanced 8-ary trees with n nodes, where a
 *          tree is balanced if the total number of nodes in subtrees
 *          corresponding to the branches of any node differ by at most one.
 * https://oeis.org/A229393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(8, r) * a(q)^(8-r) * a(q+1)^r with q = (n-1)/8, r =
 *            (n-1)%8, and a(0) = 1. This follows from the distribution of
 *            subtree sizes in a balanced 8‑ary tree
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 18, u64 n <= 83, u128 n <= 98,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

static seq_t *cache = NULL;
static idx_t size = 0;
static idx_t cap = 0;

static const idx_t C8[9] = {1,8,28,56,70,56,28,8,1};

seq_t a(idx_t n)
{
    if (n + 1 > cap) {
        /* Grow cache to hold n+1 entries */
        idx_t new_cap = n + 1;
        if (cap == 0) new_cap = n + 1;
        else if (new_cap < 2 * cap) new_cap = 2 * cap;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    if (n + 1 > size) {
        if (size == 0) {
            size = 1;
            cache[0] = 1;  /* a(0) = 1 */
        }
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }
            idx_t m = i - 1;
            idx_t q = m / 8;
            idx_t r = m % 8;

            seq_t prod = seq_pow(cache[q], 8 - r);
            if (r > 0)
                prod = prod * seq_pow(cache[q + 1], r);

            prod = prod * (seq_t)C8[r];
            cache[i] = prod;
        }
        size = n + 1;
    }

    return cache[n];
}
