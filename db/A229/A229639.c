/*
 * A229639  Number of defective 3-colorings of a 3 X n 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly two
 *          mistakes, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 15*a(n-2) - 51*a(n-3) + 96*a(n-4) + 138*a(n-5)
 *            - 92*a(n-6) - 60*a(n-7) + 48*a(n-8) - 8*a(n-9), n > 12 Initial
 *            values: a(1)=0, a(2)=39, a(3)=244, a(4)=1109, a(5)=6031,
 *            a(6)=28448, a(7)=136778, a(8)=633328, a(9)=2905016,
 *            a(10)=13118016, a(11)=58633648, a(12)=259523840
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑computed values for 1 ≤ n ≤ 12 */
    static const seq_t init[13] = {
        /* 0 unused */ 0ULL,
        0ULL,          /* a(1) */
        39ULL,         /* a(2) */
        244ULL,        /* a(3) */
        1109ULL,       /* a(4) */
        6031ULL,       /* a(5) */
        28448ULL,      /* a(6) */
        136778ULL,     /* a(7) */
        633328ULL,     /* a(8) */
        2905016ULL,    /* a(9) */
        13118016ULL,   /* a(10) */
        58633648ULL,   /* a(11) */
        259523840ULL   /* a(12) */
    };

    /* For n ≤ 12, return the pre‑computed value directly */
    if (n <= 12u)
        return init[(idx_t)n];

    /* Dynamic cache: store computed values up to the largest requested n */
    static seq_t *cache   = NULL;   /* cache[0] unused */
    static size_t cache_size = 0;   /* highest index stored + 1 (capacity) */

    /* Ensure the cache covers the requested index */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;            /* indices 0…n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();                   /* out of memory */
        cache = new_cache;
        cache_size = new_size;

        /* Initialise newly allocated entries that are ≤12 */
        for (size_t i = 1; i <= 12 && i < cache_size; ++i)
            cache[i] = init[(idx_t)i];
    }

    /* Compute from the highest already computed index + 1 up to n */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        seq_t t1 = 9 * cache[i - 1];
        seq_t t2 = 15 * cache[i - 2];
        seq_t t3 = 51 * cache[i - 3];
        seq_t t4 = 96 * cache[i - 4];
        seq_t t5 = 138 * cache[i - 5];
        seq_t t6 = 92 * cache[i - 6];
        seq_t t7 = 60 * cache[i - 7];
        seq_t t8 = 48 * cache[i - 8];
        seq_t t9 = 8 * cache[i - 9];

        /* Recurrence: a(i) = 9a(i‑1) – 15a(i‑2) – 51a(i‑3) + 96a(i‑4)
                          + 138a(i‑5) – 92a(i‑6) – 60a(i‑7) + 48a(i‑8) – 8a(i‑9) */
        cache[i] = t1 - t2 - t3 + t4 + t5 - t6 - t7 + t8 - t9;
    }

    return cache[(size_t)n];
}
