/*
 * A072884  3rd-order digital invariants: the sum of the cubes of the digits
 *          of n equals some number k and the sum of the cubes of the digits
 *          of k equals n.
 * https://oeis.org/A072884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   A072884 – k such that f(f(k)) = k, where f(k) = Σ (digit^3)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_cube_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t and idx_t are defined by the build. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_cube_sum.h"

/* Cache of the sequence */
static seq_t *table = NULL;
static size_t count  = 0;
static bool ready    = false;

/* Initialise the cache once */
static void init_cache(void)
{
    if (ready) return;

    /* All solutions lie below 3000 – this bound comfortably exceeds the
       known maximum (2188).  Using a small bound keeps the allocation tiny
       even on wide integer builds. */
    const idx_t LIMIT = 3000;
    table = (seq_t *)malloc(sizeof(seq_t) * LIMIT);
    if (!table) { ready = true; return; }

    size_t idx = 0;
    for (idx_t n = 1; n <= LIMIT; ++n) {
        seq_t s = digit_cube_sum((seq_t)n);
        seq_t t = digit_cube_sum(s);
        if (t == n) {
            table[idx++] = (seq_t)n;
        }
    }

    count = idx;
    ready = true;
}

/* Public function required by the evaluator */
seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || n > count) return 0;
    return table[n - 1];
}
