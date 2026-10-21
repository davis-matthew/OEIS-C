/*
 * A055899  Column 3 of triangle A055898.
 * https://oeis.org/A055899
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b_n + 5·a(n‑1) – 8·a(n‑2) + 14·a(n‑4) – 14·a(n‑5) +
 *            8·a(n‑7) – 5·a(n‑8) + a(n‑9) , where b_n is the numerator
 *            coefficient of the generating function: b_n = 1 (n=3), 2 (n=4),
 *            4 (n=5), 2 (n=6), 1 (n=7), 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   mul_small_const, rhs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small_const.h"
#include "../utils/rhs.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of terms stored (cache[0..cached-1]) */

    if (n >= cached) {
        /* enlarge cache up to n+1 entries */
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        for (idx_t i = cached; i <= n; ++i) {
            seq_t val = rhs(i);

            /* add positive contributions first to avoid underflow when subtracting */
            if (i >= 1) val += mul_small_const(cache[i-1], 5);
            if (i >= 4) val += mul_small_const(cache[i-4], 14);
            if (i >= 7) val += mul_small_const(cache[i-7], 8);
            if (i >= 9) val += cache[i-9];

            /* subtract negative contributions */
            if (i >= 2) val -= mul_small_const(cache[i-2], 8);
            if (i >= 5) val -= mul_small_const(cache[i-5], 14);
            if (i >= 8) val -= mul_small_const(cache[i-8], 5);

            cache[i] = val;
        }
        cached = n + 1;
    }
    return cache[n];
}
