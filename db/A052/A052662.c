/*
 * A052662  E.g.f. (1-x^2)/(1-2x-x^2+x^3).
 * https://oeis.org/A052662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n^3+6n^2+11n+6)*a(n)+(-n^2-5n-6)*a(n+1)+(-2n-6)*a(n+2)+a(n+3)=0
   Initial values: a(0)=1, a(1)=2, a(2)=8 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache          */
    static idx_t last = (idx_t)-1;  /* last index already computed     */

    /* initialise cache with first three terms on the first call */
    if (cache == NULL) {
        /* 4 entries are enough to start the recurrence */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* out‑of‑memory guard (should not happen) */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        last    = 2;
    }

    /* enlarge cache if necessary */
    if (n >= cap) {
        size_t newcap = cap;
        while (n >= newcap) newcap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                 /* out‑of‑memory guard */
        cache = tmp;
        cap   = newcap;
    }

    /* if the requested value is already cached, return it */
    if (n <= last) return cache[n];

    /* compute new values using the recurrence */
    for (idx_t i = last - 2; i <= n - 3; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t ii    = i_seq * i_seq;            /* i^2 */
        seq_t coeff1 = ii + (seq_t)(5 * i) + (seq_t)6;      /* i^2 +5i+6 */
        seq_t coeff2 = (seq_t)(2 * i) + (seq_t)6;           /* 2i+6 */

        seq_t iii = ii * i_seq;                  /* i^3 */
        seq_t coeff3 = iii + 6 * ii + 11 * i_seq + (seq_t)6; /* i^3+6i^2+11i+6 */

        seq_t term1 = cache[i + 1] * coeff1;     /* a(i+1)*(i^2+5i+6) */
        seq_t term2 = cache[i + 2] * coeff2;     /* a(i+2)*(2i+6)     */
        seq_t term3 = cache[i] * coeff3;         /* a(i)*(i^3+6i^2+11i+6) */

        seq_t sum = term1 + term2;               /* sum of positive terms */
        cache[i + 3] = sum - term3;             /* guaranteed non‑negative */
    }

    last = n;
    return cache[n];
}
