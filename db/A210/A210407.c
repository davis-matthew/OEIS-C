/*
 * A210407  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          three or four distinct values, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A210407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) + 40*a(n-2) - 280*a(n-3) + 96*a(n-4), n>=5 *
 *            Initial terms: a(1)=78, a(2)=2520, a(3)=80496, a(4)=2574592
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1, return 0 as safe default */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* size allocated in cache (>=1)   */
    static idx_t max_comp = 0;   /* largest index already computed  */

    /* first initialisation: allocate minimal space (indices 0..4) */
    if (cache == NULL) {
        capacity = 5;          /* need indices 0..4; index 0 unused */
        cache = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */

        /* hard‑code first four terms */
        cache[1] = (seq_t)78;
        cache[2] = (seq_t)2520;
        cache[3] = (seq_t)80496;
        cache[4] = (seq_t)2574592;

        max_comp = 4;
    }

    /* grow cache if requested index beyond allocated size */
    if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any terms beyond what we already have */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* recurrence: a(i) = 31*a(i-1) + 40*a(i-2) - 280*a(i-3) + 96*a(i-4) */
        cache[i] =
            (seq_t)(31 * cache[i-1] + 40 * cache[i-2] - 280 * cache[i-3] + 96 * cache[i-4]);
    }
    if (n > max_comp) max_comp = n;

    return cache[n];
}
