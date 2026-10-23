/*
 * A193127  Numbers of spanning trees of the antiprism graphs.
 * https://oeis.org/A193127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 16*a(n-1)-80*a(n-2)+130*a(n-3)-80*a(n-4)+16*a(n-5)-a(n-6)
 *            ; initial values: a(1)=2, a(2)=36, a(3)=384, a(4)=3528,
 *            a(5)=30250, a(6)=248832
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* cache[1] .. cache[max_n] */
    static idx_t max_n = 0;

    /* handle the trivial cases */
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    /* initialize cache with first 6 terms */
    if (!cache) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)384;
        cache[4] = (seq_t)3528;
        cache[5] = (seq_t)30250;
        cache[6] = (seq_t)248832;
        max_n = 6;
    }

    /* extend cache if necessary */
    if (n > max_n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* pos = 16*a[i-1] + 130*a[i-3] + 16*a[i-5] */
            seq_t pos = ((seq_t)16 * cache[i - 1]) + ((seq_t)130 * cache[i - 3]) + ((seq_t)16 * cache[i - 5]);
            /* neg = 80*a[i-2] + 80*a[i-4] + a[i-6] */
            seq_t neg = ((seq_t)80 * cache[i - 2]) + ((seq_t)80 * cache[i - 4]) + cache[i - 6];
            cache[i] = pos - neg;
        }
        max_n = n;
    }

    return cache[n];
}
