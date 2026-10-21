/*
 * A205991  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two, and every 2 X 2
 *          determinant nonzero.
 * https://oeis.org/A205991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence a(n) = 2*a(n‑1) + 5*a(n‑2) + 3*a(n‑3) –
 *            2*a(n‑4) + a(n‑5) initial values: a(1)=26, a(2)=92, a(3)=330,
 *            a(4)=1184, a(5)=4250 (OEIS offset is 1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* static cache of previously computed values (1‑based) */
    static seq_t *cache = (seq_t *)0;
    static idx_t cache_size = 0;

    /* initialise cache with the first five terms if needed */
    if (!cache) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));   /* indices 0..5, ignore index 0 */
        if (!cache) return (seq_t)0;                  /* allocation failed */
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)92;
        cache[3] = (seq_t)330;
        cache[4] = (seq_t)1184;
        cache[5] = (seq_t)4250;
        cache_size = 5;
    }

    /* if the requested value is already cached, return it immediately */
    if (n <= cache_size) return cache[n];

    /* grow the cache to hold up to index n */
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;            /* allocation failed – return 0 as error */
    cache = tmp;

    /* compute the new terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = 2 * a1 + 5 * a2 + 3 * a3 - 2 * a4 + a5;
    }
    cache_size = n;
    return cache[n];
}
