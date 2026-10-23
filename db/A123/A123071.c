/*
 * A123071  Bishops on a 2n+1 X 2n+1 board (see Robinson paper for details).
 * https://oeis.org/A123071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to place n non‑attacking Bishops on a
 *            (2n+1)×(2n+1) board
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (conjectured): 2*a(n) + a(n-1) -2*n*a(n-2) +(-n-10)*a(n-3) -2*(n-2)*(n+2)*a(n-4)
   +(-n^2-2*n+23)*a(n-5) +2*(n-5)*(n^2-7*n+11)*a(n-6) +(n-6)*(n-5)^2*a(n-7)=0,
   with initial values a(0)=1,a(1)=2,a(2)=4,a(3)=12,a(4)=36,a(5)=120,a(6)=400. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache with first 7 terms */
    if (cache == NULL) {
        cache_cap = 8;                           /* at least 7 values */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)36;
        cache[5] = (seq_t)120;
        cache[6] = (seq_t)400;
        cache_len = 7;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache to hold up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 6)               /* base values already set */
            continue;

        seq_t a_n1 = cache[i - 1];
        seq_t a_n2 = cache[i - 2];
        seq_t a_n3 = cache[i - 3];
        seq_t a_n4 = cache[i - 4];
        seq_t a_n5 = cache[i - 5];
        seq_t a_n6 = cache[i - 6];
        seq_t a_n7 = cache[i - 7];

        seq_t NN = (seq_t)i;

        /* positive part of the RHS of the recurrence */
        seq_t pos =
            ((seq_t)2 * NN * a_n2) +
            ((NN + (seq_t)10) * a_n3) +
            ((seq_t)2 * (NN - (seq_t)2) * (NN + (seq_t)2) * a_n4) +
            ((NN * NN + (seq_t)2 * NN - (seq_t)23) * a_n5);

        /* negative part of the RHS of the recurrence */
        seq_t neg =
            a_n1 +
            ((seq_t)2 * (NN - (seq_t)5) *
             (NN * NN - (seq_t)7 * NN + (seq_t)11) * a_n6) +
            ((NN - (seq_t)6) * (NN - (seq_t)5) *
             (NN - (seq_t)5) * a_n7);

        seq_t rhs = pos - neg;          /* should be non‑negative */
        seq_t a_n_val = rhs / 2;

        cache[i] = a_n_val;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
