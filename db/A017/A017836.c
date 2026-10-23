/*
 * A017836  Expansion of
 *          1/(1-x^4-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+
 *            a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14) Initial values: a(0)=1,
 *            a(1)=a(2)=a(3)=0; a(4)=1, a(5)=1, a(6)=1, a(7)=1, a(8)=2,
 *            a(9)=3, a(10)=4, a(11)=5, a(12)=7, a(13)=10, a(14)=14
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 75, u64 n <= 145,
 *            u128 n <= 285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cached_n = 0;   /* highest index computed */
    static size_t cap = 0;        /* allocated array size */

    /* initial allocation and seed of first 15 terms */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure – unlikely in tests */

        /* seed with known values */
        cache[0] = 1;
        cache[1] = 0; cache[2] = 0; cache[3] = 0;
        cache[4] = 1; cache[5] = 1; cache[6] = 1; cache[7] = 1;
        cache[8] = 2; cache[9] = 3; cache[10] = 4; cache[11] = 5;
        cache[12] = 7; cache[13] = 10; cache[14] = 14;
        cached_n = 14;
    }

    /* guard against ridiculously large n that would overflow size_t */
    if (n > (idx_t)SIZE_MAX) return 0;

    size_t N = (size_t)n;

    /* grow cache if necessary */
    if (N > cached_n) {
        /* enlarge capacity to at least N+1 */
        if (N >= cap) {
            size_t new_cap = (cap == 0) ? 16 : cap;
            while (new_cap <= N) new_cap <<= 1;      /* double until sufficient */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;  /* allocation failure */
            /* zero the newly allocated part */
            for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
            cache = tmp;
            cap = new_cap;
        }
        /* compute missing terms */
        for (size_t i = cached_n + 1; i <= N; ++i) {
            seq_t sum = 0;
            if (i >= 4) sum += cache[i-4];
            if (i >= 5) sum += cache[i-5];
            if (i >= 6) sum += cache[i-6];
            if (i >= 7) sum += cache[i-7];
            if (i >= 8) sum += cache[i-8];
            if (i >= 9) sum += cache[i-9];
            if (i >= 10) sum += cache[i-10];
            if (i >= 11) sum += cache[i-11];
            if (i >= 12) sum += cache[i-12];
            if (i >= 13) sum += cache[i-13];
            if (i >= 14) sum += cache[i-14];
            cache[i] = sum;
        }
        cached_n = N;
    }

    return cache[N];
}
