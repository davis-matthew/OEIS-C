/*
 * A028479  Number of perfect matchings in graph C_{8} X P_{n}.
 * https://oeis.org/A028479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028479: Number of perfect matchings in graph C₈ × Pₙ
 * Recurrence (for n ≥ 9):
 *   a(n) = 7*a(n-1) + 56*a(n-2) - 104*a(n-3) - 280*a(n-4)
 *          + 280*a(n-5) + 104*a(n-6) - 56*a(n-7) - 7*a(n-8) + a(n-9)
 * Initial values (n = 0 … 8):
 *   a(0)=1, a(1)=2, a(2)=49, a(3)=392, a(4)=5041,
 *   a(5)=51842, a(6)=591361, a(7)=6422528, a(8)=71385601
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements allocated */

    /* Ensure cache contains at least n+1 elements.
     * The first 9 terms are always required for the recurrence. */
    if (!cache) {
        idx_t need = (n + 1 > 9) ? (n + 1) : 9;
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) exit(1);

        /* Initialize the first 9 terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)392;
        cache[4] = (seq_t)5041;
        cache[5] = (seq_t)51842;
        cache[6] = (seq_t)591361;
        cache[7] = (seq_t)6422528;
        cache[8] = (seq_t)71385601;

        cache_size = need;
    } else if (cache_size <= n) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t t0 = (seq_t)7 * cache[i-1];
            seq_t t1 = (seq_t)56 * cache[i-2];
            seq_t t2 = (seq_t)104 * cache[i-3];
            seq_t t3 = (seq_t)280 * cache[i-4];
            seq_t t4 = (seq_t)280 * cache[i-5];
            seq_t t5 = (seq_t)104 * cache[i-6];
            seq_t t6 = (seq_t)56 * cache[i-7];
            seq_t t7 = (seq_t)7 * cache[i-8];

            seq_t positives = t0 + t1 + t4 + t5 + cache[i-9];
            seq_t negatives = t2 + t3 + t6 + t7;
            cache[i] = positives - negatives;   /* final value */
        }
    }

    return cache[n];
}
