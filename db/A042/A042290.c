/*
 * A042290  Numerators of continued fraction convergents to sqrt(671).
 * https://oeis.org/A042290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 117240*a(n-10) - a(n-20), with initial values: a(0)=25,
 *            a(1)=26, a(2)=259, a(3)=544, a(4)=803, a(5)=1347, a(6)=2150,
 *            a(7)=5647, a(8)=52973, a(9)=58620, a(10)=2983973, a(11)=3042593,
 *            a(12)=30367310, a(13)=63777213, a(14)=94144523, a(15)=157921736,
 *            a(16)=252066259, a(17)=662054254, a(18)=621...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for computed terms */
    static seq_t *cache = NULL;      /* dynamic array of terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_len = 0;      /* number of terms stored (>=1) */
    static int init_done = 0;        /* flag whether initial 20 terms are ready */
    static seq_t init_terms[20];

    /* ------------------------------------------------------------------ */
    /* Phase 1: compute the first 20 numerators from the continued‑fraction
       expansion of sqrt(671) (if not already done).                         */
    if (!init_done) {
        /* integer floor(sqrt(671)) using a simple loop */
        const int D = 671;
        int a0 = 0;
        while ((long long)(a0 + 1) * (a0 + 1) <= D)
            a0++;

        /* variables for the continued‑fraction algorithm */
        int m = 0;
        int d = 1;
        int a = a0;     /* current partial quotient */
        seq_t p_prev2 = 0;
        seq_t p_prev1 = 1;   /* p_-1 */
        for (int k = 0; k < 20; ++k) {
            seq_t p = (seq_t)a * p_prev1 + p_prev2;
            init_terms[k] = p;

            /* shift for next step */
            p_prev2 = p_prev1;
            p_prev1 = p;

            /* update CF variables */
            m = d * a - m;
            d = (D - (long long)m * m) / d;
            a = (a0 + m) / d;
        }

        /* allocate the cache and fill in the initial 20 terms */
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* it is safe to assume malloc succeeds for the expected sizes */
        for (size_t i = 0; i < 20; ++i)
            cache[i] = init_terms[i];
        cache_len = 20;
        init_done = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Phase 2: ensure cache is large enough for requested n */
    if ((idx_t)cache_len <= n) {
        /* Grow capacity (doubling strategy). */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;                /* multiply by 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence.  We start at i=cache_len
           which is always >= 20 after the initialization step. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)117240 * cache[i - 10] - cache[i - 20];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
