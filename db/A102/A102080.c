/*
 * A102080  Number of matchings in the C_n X P_2 (n-prism) graph.
 * https://oeis.org/A102080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=12, a(3)=32, a(4)=108,
 *            a(n)=2*a(n-1)+4*a(n-2)-a(n-4) for n>4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size  = 0;

    if (n == 0) return 0;  /* not defined in OEIS, return 0 */

    if (cache == NULL) {
        size = 4;
        cache = (seq_t*)malloc((size + 1) * sizeof(seq_t));
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)32;
        cache[4] = (seq_t)108;
    }

    if (n <= size) return cache[n];

    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));

    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)2
                 + cache[i - 2] * (seq_t)4
                 - cache[i - 4];
        cache[i] = val;
    }

    size = n;
    return cache[n];
}
