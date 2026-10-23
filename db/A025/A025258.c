/*
 * A025258  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 1, -1, 1, 1.
 * https://oeis.org/A025258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1}^{n-3} a(k)*a(n-k) initial values: a(1)=1,
 *            a(2)=-1, a(3)=1, a(4)=1
 * Range:     i8 n <= 16, i16 n <= 25, i32 n <= 44, i64 n <= 79, i128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] unused, cache[1..] hold computed values   */
    static seq_t *cache = NULL;   /* NULL indicates uninitialized   */
    static idx_t cached_len = 0;  /* highest n for which cache[n] is valid */

    /* ---------- initialisation ---------- */
    if (cache == NULL) {
        /* allocate space for indices 0..4 (5 slots, 0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 5U);
        if (!cache) return (seq_t)0;  /* out of memory – unlikely in tests */

        cache[0] = (seq_t)0;   /* dummy */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cached_len = 4;
    }

    /* ---------- requested term already cached? ---------- */
    if (n <= cached_len) return cache[(size_t)n];

    /* ---------- extend cache up to n ---------- */
    /* realloc to size n+1 to accommodate new index */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1U));
    if (!new_cache) return (seq_t)0;  /* out of memory – unlikely in tests */

    cache = new_cache;

    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* recurrence valid for i >= 5: sum k=1..i-3 */
        if (i >= 5) {
            for (idx_t k = 1; k <= i - 3; ++k) {
                /* array indices fit into size_t */
                seq_t left  = cache[(size_t)k];
                seq_t right = cache[(size_t)(i - k)];
                sum += left * right;
            }
        } else {
            /* i is between 1 and 4 – already initialized */
            sum = cache[(size_t)i];
        }
        cache[(size_t)i] = sum;
    }

    cached_len = n;
    return cache[(size_t)n];
}
