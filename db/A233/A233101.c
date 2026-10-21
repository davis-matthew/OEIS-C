/*
 * A233101  Number of 4Xn 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, diagonally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) + 52*a(n-2) - 1084*a(n-3) + 16*a(n-4) +
 *            9754*a(n-5) * - 16165*a(n-6) + 5229*a(n-7) + 5498*a(n-8) -
 *            3740*a(n-9) + 320*a(n-10) + 96*a(n-11) * Initial values (for 1 ≤
 *            n ≤ 12): 36, 515, 13721, 342626, 8714705, 221113913, *
 *            5614356176, 142541243171, 3619052592419, 91885574325470, * 23...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t A233101_BASE[13] = {
    0,  /* index 0 unused */
    36ULL, 515ULL, 13721ULL, 342626ULL,
    8714705ULL, 221113913ULL, 5614356176ULL,
    142541243171ULL, 3619052592419ULL,
    91885574325470ULL, 2332922952059027ULL,
    59231586841647287ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */

    static seq_t *cache = NULL;           /* 1‑based cache */
    static idx_t cap   = 0;               /* capacity of cache (highest index + 1) */

    /* Grow cache if needed. New elements are zeroed. */
    if (n + 1 > cap) {
        idx_t new_cap = cap == 0 ? 16 : cap * 2;
        if (new_cap <= n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – return 0 */
        cache = tmp;
        /* zero the new part of the array */
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Initialize base terms only once */
    if (cache[1] == 0) {
        for (idx_t i = 1; i <= 12; ++i) cache[i] = A233101_BASE[i];
    }

    /* Compute terms beyond the 12th using the recurrence. */
    for (idx_t i = 13; i <= n; ++i) {
        if (cache[i] != 0) continue;  /* already computed */

        /* All calculations are done in seq_t. For larger seq_t this
           will not overflow. If seq_t is small, overflow is OK for n
           where the final value still fits in seq_t. */
        seq_t val = 0;
        val += (seq_t)25   * cache[i-1];
        val += (seq_t)52   * cache[i-2];
        val += (seq_t)16   * cache[i-4];
        val += (seq_t)9754 * cache[i-5];
        val += (seq_t)5229 * cache[i-7];
        val += (seq_t)5498 * cache[i-8];
        val += (seq_t)320  * cache[i-10];
        val += (seq_t)96   * cache[i-11];

        val -= (seq_t)1084 * cache[i-3];
        val -= (seq_t)16165 * cache[i-6];
        val -= (seq_t)3740 * cache[i-9];

        cache[i] = val;
    }

    return cache[n];
}
