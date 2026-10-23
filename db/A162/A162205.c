/*
 * A162205  Number of reduced words of length n in the Weyl group B_50.
 * https://oeis.org/A162205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in Product_{k=1..50} (1 - x^(2k)) / (1
 *            - x)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types: seq_t, idx_t are defined by the build system */

static seq_t *a_cache = NULL;   /* memoized values up to n = 2500 */
static size_t a_cached_len = 0; /* length of a_cache (2501) */

/* Helper: allocate an array of seq_t */
static seq_t* alloc_seq_array(size_t n)
{
    /* cast to void* to silence strict aliasing warnings */
    return (seq_t*)malloc(n * sizeof(seq_t));
}

/* Function to compute the sequence value */
seq_t a(idx_t n)
{
    /* Max length for B_50: 50^2 = 2500 (after that all terms are zero) */
    const idx_t M = 50;
    const idx_t MAX_N = M * M;          /* 2500 */
    const int DEG = M * (M + 1);        /* 2550 */

    /* Ensure memoization table is computed once */
    if (a_cache == NULL || a_cached_len != (size_t)(MAX_N + 1))
    {
        /* Allocate polynomial coefficients for P(x) = ∏(1 - x^(2k)) */
        int *coeff = (int*)malloc((DEG + 1) * sizeof(int));
        for (int i = 0; i <= DEG; ++i) coeff[i] = 0;
        coeff[0] = 1;
        int cur_deg = 0;

        for (int k = 1; k <= M; ++k)
        {
            int step = 2 * k;
            /* Multiply by (1 - x^step) : coeff[j+step] -= coeff[j] */
            for (int j = cur_deg; j >= 0; --j)
            {
                coeff[j + step] -= coeff[j];
            }
            cur_deg += step; /* new degree after multiplication */
        }

        /* Pre‑compute binomial coefficients C(n + M - 1, M - 1)
         * for n = 0..MAX_N.  We need values up to MAX_N + M - 1.
         */
        const idx_t BINOM_MAX = MAX_N + M - 1; /* 2500 + 49 = 2549 */
        seq_t *binom = alloc_seq_array((size_t)(BINOM_MAX + 1));
        for (idx_t i = 0; i <= BINOM_MAX; ++i) binom[i] = 0;

        const idx_t START = M - 1; /* 49 */
        if (BINOM_MAX >= START)
        {
            binom[START] = (seq_t)1; /* C(49,49) = 1 */
            for (idx_t t = START + 1; t <= BINOM_MAX; ++t)
            {
                /* binom[t] = binom[t-1] * t / (t - START) */
                seq_t num = binom[t - 1] * (seq_t)t;
                binom[t] = num / (seq_t)(t - START);
            }
        }

        /* Allocate cache for a(n) */
        a_cache = alloc_seq_array((size_t)(MAX_N + 1));
        for (idx_t i = 0; i <= MAX_N; ++i)
        {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            int max_j = (i <= (idx_t)DEG) ? (int)i : DEG;
            for (int j = 0; j <= max_j; ++j)
            {
                int c = coeff[j];
                if (c == 0) continue;
                /* index into binom array: n - j + M - 1 */
                idx_t idx_b = i - (idx_t)j + (idx_t)(M - 1);
                seq_t val = binom[idx_b];
                if (c > 0)
                    sum_pos += val;
                else
                    sum_neg += val; /* c == -1 */
            }
            a_cache[i] = sum_pos - sum_neg;
        }

        free(coeff);
        free(binom);
        a_cached_len = (size_t)(MAX_N + 1);
    }

    if (n <= (idx_t)MAX_N)
        return a_cache[(size_t)n];
    else
        return (seq_t)0;
}
