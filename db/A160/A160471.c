/*
 * A160471  Fourth right hand column of triangles A094665 and A083061.
 * https://oeis.org/A160471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 34 a(n+1) =
 *            ((70*n^4+1407*n^3+10520*n^2+34707*n+42660)*a(n)) /
 *            (35*n^3+336*n^2+913*n+612)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A160471. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* First term initialization. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = (seq_t)34;
        cache_size = 1;
    }

    /* Return cached value if available. */
    if (n < cache_size) return cache[n];

    /* Resize cache to accommodate up to index n. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) return 0;               /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size - 1; i < n; ++i) {
        /* Polynomial components.  Use unsigned 64‑bit arithmetic – safe
           for i ≤ 10000. */
        unsigned long long i_u = (unsigned long long)i;
        unsigned long long i2  = i_u * i_u;          /* i^2   */
        unsigned long long i3  = i2 * i_u;          /* i^3   */
        unsigned long long i4  = i3 * i_u;          /* i^4   */

        unsigned long long num = 70ULL * i4
                               + 1407ULL * i3
                               + 10520ULL * i2
                               + 34707ULL * i_u
                               + 42660ULL;

        unsigned long long den = 35ULL * i3
                               + 336ULL * i2
                               + 913ULL * i_u
                               + 612ULL;

        seq_t prev = cache[i];

        /* Compute (prev * num) / den using 128‑bit intermediate.
           This guarantees no overflow for built‑in types up to 64 bits. */
        __uint128_t prod = (__uint128_t)prev * (__uint128_t)num;
        seq_t next = (seq_t)(prod / den);

        cache[i + 1] = next;
    }

    cache_size = n + 1;
    return cache[n];
}
