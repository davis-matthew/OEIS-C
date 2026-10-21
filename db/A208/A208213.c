/*
 * A208213  a(n)=(a(n-1)^3*a(n-2)^2+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^3 * a(n-2)^2 + 1) / a(n-3) a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static void ensure_capacity(size_t need, seq_t **p, size_t *cap) {
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap << 1) : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    *p = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    if (!cache) {
        size_t init = 3;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cap = init;
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        len = 3;
    }

    if ((size_t)n >= len) {
        size_t target = (size_t)n;
        ensure_capacity(target + 1, &cache, &cap);
        for (size_t i = len; i <= target; ++i) {
            seq_t ai1 = cache[i - 1];
            seq_t ai2 = cache[i - 2];
            seq_t ai3 = cache[i - 3];

            seq_t cube   = ai1 * ai1 * ai1;   /* (a(i-1))^3 */
            seq_t square = ai2 * ai2;          /* (a(i-2))^2 */
            seq_t prod   = cube * square;      /* product          */
            prod = prod + 1;                   /* + 1              */
            cache[i] = prod / ai3;             /* exact division   */
        }
        len = target + 1;
    }

    return cache[(size_t)n];
}
