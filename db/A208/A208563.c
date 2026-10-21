/*
 * A208563  Number of nX4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(n) = 61*a(n-1) -1124*a(n-2) +9139*a(n-3)
 *                -34187*a(n-4) +44684*a(n-5) +35628*a(n-6)
 *                -128720*a(n-7) +102352*a(n-8) -44832*a(n-9) +17472*a(n-10)
 * For n > 11.  Initial values are a(1) … a(11) given in the problem.
 * The function memoises computed terms and uses only + - * on seq_t.
 */

#include <stdlib.h>

static const int COEF[10] = {
    61,    -1124, 9139, -34187, 44684,
    35628, -128720, 102352, -44832, 17472
};

/* a(0) is unused; a(1) … a(11) are stored. */
static const seq_t INIT[12] = {
    0ULL,               /* unused a(0) */
    14ULL,              /* a(1) */
    464ULL,             /* a(2) */
    16608ULL,           /* a(3) */
    600426ULL,          /* a(4) */
    21758476ULL,        /* a(5) */
    788922798ULL,       /* a(6) */
    28608436530ULL,     /* a(7) */
    1037446303548ULL,   /* a(8) */
    37621813069386ULL,  /* a(9) */
    1364314106628550ULL,/* a(10) */
    49475379592575628ULL/* a(11) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1 */

    /* lazy static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;     /* highest index computed (1…11 initially) */

    if (cache_size == 0) {
        const size_t init_cap = 12;   /* indices 0…11 inclusive */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        for (size_t i = 0; i < 12; ++i) cache[i] = INIT[i];
        cache_size = 11;             /* we have values up to a(11) */
    }

    if (n <= cache_size) return cache[(size_t)n];

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t pos = 0;   /* positive contributions */
        seq_t neg = 0;   /* absolute value of negative contributions */
        for (int i = 0; i < 10; ++i) {
            idx_t idx = k - 1 - (idx_t)i;   /* term a(k-i-1) */
            seq_t prod = cache[idx] * (seq_t)(COEF[i] >= 0 ? COEF[i] : -COEF[i]);
            if (COEF[i] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        /* final value is positive as the sequence counts arrays */
        cache[(size_t)k] = pos - neg;
    }

    cache_size = n;
    return cache[(size_t)n];
}
