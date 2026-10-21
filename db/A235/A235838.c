/*
 * A235838  Number of (n+1)X(1+1) 0..2 arrays with the sum of each 2X2
 *          subblock two extreme terms minus its two median terms
 *          lexicographically nondecreasing rowwise and columnwise.
 * https://oeis.org/A235838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -204*a(n-3) +368*a(n-4) +1728*a(n-5)
 *            -4720*a(n-6) -6672*a(n-7) +25984*a(n-8) +10560*a(n-9)
 *            -74880*a(n-10) +1920*a(n-11) +116160*a(n-12) -24000*a(n-13)
 *            -89600*a(n-14) +19200*a(n-15) +25600*a(n-16) a(1)=81, a(2)=487,
 *            a(3)=2581, a(4)=12373, a(5)=55929, a(6)=241933, a(7)=1015889,
 *            a(8)...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#define INIT_COUNT 16
static const seq_t init_vals[INIT_COUNT] = {
    (seq_t)81ULL,
    (seq_t)487ULL,
    (seq_t)2581ULL,
    (seq_t)12373ULL,
    (seq_t)55929ULL,
    (seq_t)241933ULL,
    (seq_t)1015889ULL,
    (seq_t)4174941ULL,
    (seq_t)16895401ULL,
    (seq_t)67647573ULL,
    (seq_t)268781649ULL,
    (seq_t)1062687069ULL,
    (seq_t)4187377529ULL,
    (seq_t)16469997477ULL,
    (seq_t)64715520737ULL,
    (seq_t)254249916141ULL
};

static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* the sequence is defined for n>=1 only */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;   /* largest index that has been computed */

    if (cache == NULL) {
        ensure_capacity(INIT_COUNT + 1, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)0;  /* unused index 0 */
        for (size_t i = 0; i < INIT_COUNT; ++i)
            cache[i + 1] = init_vals[i];
        len = INIT_COUNT;
    }

    if (len >= (size_t)n)
        return cache[n];

    ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));

    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        pos += ((seq_t)9)      * cache[i - 1];
        pos += ((seq_t)368)    * cache[i - 4];
        pos += ((seq_t)1728)   * cache[i - 5];
        pos += ((seq_t)25984)  * cache[i - 8];
        pos += ((seq_t)10560)  * cache[i - 9];
        pos += ((seq_t)1920)   * cache[i - 11];
        pos += ((seq_t)116160) * cache[i - 12];
        pos += ((seq_t)19200)  * cache[i - 15];
        pos += ((seq_t)25600)  * cache[i - 16];

        neg += ((seq_t)204)    * cache[i - 3];
        neg += ((seq_t)4720)   * cache[i - 6];
        neg += ((seq_t)6672)   * cache[i - 7];
        neg += ((seq_t)74880)  * cache[i - 10];
        neg += ((seq_t)24000)  * cache[i - 13];
        neg += ((seq_t)89600)  * cache[i - 14];

        cache[i] = pos - neg;
    }

    len = (size_t)n;
    return cache[n];
}
