/*
 * A141683  a(n) = Sum_{k=1..n} b(k)*a(n - k) for n >= 1, where b(n) = b(n-2)
 *          + b(n-3) for n >= 3 with b(0) = 0 and b(1) = b(2) = 1.
 * https://oeis.org/A141683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) for n > 4 Initial values:
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=4 (The sequence also equals
 *            A000930(2*n-3) for n≥3)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: a[1]..a[cached_size] */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    if (n < 1) return 0;            /* ignore requests below offset */

    if (cache == NULL) {            /* initial allocation */
        size_t init_sz = 10;        /* enough for indices up to 9 */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_sz);
        if (!cache) return 0;       /* allocation failed, return 0 */
        cached_size = 0;
        /* base cases */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 4;
        cached_size = 4;            /* we have computed up to 4 */
    }

    /* expand cache if needed */
    if (n > cached_size) {
        size_t new_sz = (size_t)(n) + 5;  /* some spare room */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_sz);
        if (!tmp) return 0;             /* realloc failed */
        cache = tmp;
        for (idx_t i = cached_size + 1; i <= n; ++i) {
            /* recurrence: a[i] = a[i-1] + 2*a[i-2] + a[i-3] */
            cache[i] = cache[i-1] + 2 * cache[i-2] + cache[i-3];
        }
        cached_size = n;
    }

    return cache[n];
}
