/*
 * A219074  Number of n X 4 binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 33*a(n-2) + 233*a(n-3) + 13*a(n-4) -
 *            31*a(n-5) - 657*a(n-6) - 321*a(n-7) - 288*a(n-8) - 64*a(n-9) +
 *            64*a(n-10) for n ≥ 11; Initial values: a(1)=9, a(2)=165,
 *            a(3)=2321, a(4)=32945, a(5)=477309, a(6)=6879341, a(7)=99118753,
 *            a(8)=1428782305, a(9)=20594013941, a(10)=296830835781
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1, harmless */

    /* Static cache of computed values. Index 0 unused. */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size */
    static idx_t last = 0;      /* largest n already computed */

    /* Ensure capacity for indices up to n. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
        /* Initialize newly allocated part to zero. */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
    }

    /* Initialize the first 10 terms if not already done. */
    if (last < 10) {
        static const seq_t init[10] = {
            (seq_t)9ULL,
            (seq_t)165ULL,
            (seq_t)2321ULL,
            (seq_t)32945ULL,
            (seq_t)477309ULL,
            (seq_t)6879341ULL,
            (seq_t)99118753ULL,
            (seq_t)1428782305ULL,
            (seq_t)20594013941ULL,
            (seq_t)296830835781ULL
        };
        size_t upto = n < 10 ? (size_t)n : 10;
        for (size_t i = 1; i <= upto; ++i) cache[i] = init[i-1];
        last = upto;
    }

    /* Compute terms from last+1 up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t v = 0;
        v += cache[idx-1] * (seq_t)11;
        v += cache[idx-2] * (seq_t)33;
        v += cache[idx-3] * (seq_t)233;
        v += cache[idx-4] * (seq_t)13;
        v -= cache[idx-5] * (seq_t)31;
        v -= cache[idx-6] * (seq_t)657;
        v -= cache[idx-7] * (seq_t)321;
        v -= cache[idx-8] * (seq_t)288;
        v -= cache[idx-9] * (seq_t)64;
        v += cache[idx-10] * (seq_t)64;
        cache[idx] = v;
    }

    last = n;
    return cache[(size_t)n];
}
