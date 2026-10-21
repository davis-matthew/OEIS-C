/*
 * A057677  a(n) is the numerator of b(n) where b(n)=1/b(n-1)+1/b(n-2) with
 *          b(1)=1 and b(2)=2.
 * https://oeis.org/A057677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n-2)^2*a(n-3) + a(n-1)*a(n-3)*a(n-4) for n>=6 * Initial
 *            values: a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=32
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to avoid recomputation */
    static seq_t *cache = NULL;
    static size_t csize  = 0;   /* number of terms currently stored */
    static size_t cap    = 0;   /* allocated capacity */

    /* helper to allocate / grow the cache */
    if (csize == 0) {
        /* initialise with first 5 terms */
        cap = 8;                          /* start with a small capacity */
        cache = (seq_t*) malloc(cap * sizeof(seq_t));
        if (!cache) exit(1); /* allocation failed */
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)2;  /* a(2) */
        cache[2] = (seq_t)3;  /* a(3) */
        cache[3] = (seq_t)7;  /* a(4) */
        cache[4] = (seq_t)32; /* a(5) */
        csize = 5;
    }

    if (n <= csize) {
        return cache[n-1];
    }

    if ((size_t)n > cap) {
        /* grow capacity (double strategy) */
        size_t newcap = (cap * 2 > (size_t)n) ? cap * 2 : (size_t)n;
        seq_t *tmp = (seq_t*) realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = newcap;
    }

    /* compute terms from csize+1 up to n */
    for (size_t i = csize + 1; i <= (size_t)n; ++i) {
        /* indices in zero‑based array */
        seq_t a_nm1 = cache[i-2];   /* a(i-1)  */
        seq_t a_nm2 = cache[i-3];   /* a(i-2)  */
        seq_t a_nm3 = cache[i-4];   /* a(i-3)  */
        seq_t a_nm4 = cache[i-5];   /* a(i-4)  */

        /* term = a_nm2^2 * a_nm3 + a_nm1 * a_nm3 * a_nm4 */
        seq_t t1 = a_nm2 * a_nm2;
        t1 = t1 * a_nm3;

        seq_t t2 = a_nm1 * a_nm3;
        t2 = t2 * a_nm4;

        cache[i-1] = t1 + t2;
    }

    csize = (size_t)n;
    return cache[n-1];
}
