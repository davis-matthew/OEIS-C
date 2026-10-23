/*
 * A119401  a(n) = Sum_{k=0..n} (-1)^(n-k)*(n!/k!)^2*binomial(n,k).
 * https://oeis.org/A119401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * ((n!/k!)^2) * binomial(n,k)
 *            Recurrence: a(0)=1, a(1)=0, a(2)=-3, a(n) = -3*(n-1)*n*a(n-1) -
 *            3*(n-1)^4*a(n-2) - (n-2)^3*(n-1)^3*a(n-3) for n>=3
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static idx_t  cache_cap = 0;     /* allocated size */
static idx_t  computed_max = 0;  /* highest index already populated */

seq_t a(idx_t n)
{
    /* Expand cache to cover indices 0..n (need at least 3 slots for the base terms) */
    idx_t needed = n + 1;
    if (needed < 3) needed = 3;
    if (cache_cap < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;  /* unlikely, but keep type compatible */
        /* Initialize the freshly allocated slots to zero (not strictly needed for big ints) */
        for (idx_t i = cache_cap; i < needed; ++i) new_cache[i] = 0;
        cache      = new_cache;
        cache_cap  = needed;
    }

    /* Initialize base terms if not yet done */
    if (computed_max < 2) {
        /* a(0) = 1, a(1) = 0, a(2) = -3 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-3;
        computed_max = 2;
    }

    /* Compute terms up to n */
    while (computed_max < n) {
        ++computed_max;           /* next index to compute */
        if (computed_max <= 2) continue;   /* already set */

        idx_t i = computed_max;
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        seq_t n_minus_1 = (seq_t)(i - 1);
        seq_t n_minus_2 = (seq_t)(i - 2);
        seq_t i_seq     = (seq_t)i;

        /* (n-1)^2, (n-1)^4, (n-1)^3 */
        seq_t n1_sq   = n_minus_1 * n_minus_1;
        seq_t n1_qua  = n1_sq * n1_sq;          /* (n-1)^4 */
        seq_t n1_cub  = n1_sq * n_minus_1;      /* (n-1)^3 */

        /* (n-2)^3 */
        seq_t n2_cub  = n_minus_2 * n_minus_2 * n_minus_2;

        /* positive components of the recurrence */
        seq_t pos1 = (seq_t)3 * n_minus_1 * i_seq * a1;       /* 3*(n-1)*n*a(n-1) */
        seq_t pos2 = (seq_t)3 * n1_qua * a2;                 /* 3*(n-1)^4*a(n-2) */
        seq_t pos3 = n2_cub * n1_cub * a3;                   /* (n-2)^3*(n-1)^3*a(n-3) */

        seq_t sum = pos1 + pos2 + pos3;
        cache[i] = -sum;   /* a(n) = -sum */
    }

    return cache[n];
}
