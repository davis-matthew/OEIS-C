/*
 * A074549  a(n) = 3^n + 4^n + 7^n.
 * https://oeis.org/A074549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 4^n + 7^n. Compute powers iteratively, caching
 *            results so each value is computed only once. The algorithm is
 *            O(max(n)) over a sequence of calls and uses only integer
 *            arithmetic
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow3 = NULL;
static seq_t *pow4 = NULL;
static seq_t *pow7 = NULL;
static idx_t cache_size = 0;          /* number of pre‑computed entries (index+1) */

/* extend cached powers so that index n is available */
static void extend_to(idx_t n)
{
    if (n < cache_size) return;

    /* enlarge capacity, at least doubling */
    idx_t new_cap = cache_size ? cache_size << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;

    pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    pow4 = (seq_t *)realloc(pow4, new_cap * sizeof(seq_t));
    pow7 = (seq_t *)realloc(pow7, new_cap * sizeof(seq_t));

    /* initialise from the previous size up to new_cap-1 */
    if (cache_size == 0) {
        pow3[0] = (seq_t)1;
        pow4[0] = (seq_t)1;
        pow7[0] = (seq_t)1;
        cache_size = 1;
    }
    for (idx_t i = cache_size; i < new_cap; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
        pow4[i] = pow4[i - 1] * (seq_t)4;
        pow7[i] = pow7[i - 1] * (seq_t)7;
    }
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    extend_to(n);
    return pow3[n] + pow4[n] + pow7[n];
}
