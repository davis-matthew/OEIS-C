/*
 * A192247  1-sequence of reduction of tetrahedral number sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficients of rational function * * G(x) = x^2(2 - x)(2
 *            - 2x + 3x^2) / ((1 - x)(1 - x - x^2)^4) * * A recurrence
 *            Relation of order 9 with initial values * * a(0) = 0, a(1) = 0,
 *            * a(2) = 4, * a(3) = 14, * a(4) = 54, * a(5) = 159, * a(6) =
 *            439, * a(7) = 1111, … (see OEIS A192247) * * For n...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised implementation of a(n) for the 1‑sequence reduction of
 * the tetrahedral numbers by the substitution x² → x+1. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t computed_max = 0;      /* largest n for which cache[n] is valid */

    /* ------------------------------------------------------------------ */
    /*  Initialise recurrence coefficients once.  The coefficients are
     *  stored in signed 64‑bit integers.  They are the same for all
     *  builds and are independent of seq_t.                            */
    /* ------------------------------------------------------------------ */
    static int  coeff[10] = {0};   /* q_k from Q(x) expansion */
    static int  p_term[10] = {0};  /* p(n) from numerator N(x) */
    static int  init_done  = 0;

    if (!init_done) {
        /* Coefficients of Q(x) = (1 - x)(1 - x - x^2)^4  */
        /* First compute (1 - x - x^2)^4   */
        int f[3] = {1, -1, -1};     /* 1 - x - x^2   */
        int temp[10] = {0};
        int deg = 0;
        temp[0] = 1;                /* initial polynomial = 1 */
        for (int mult = 0; mult < 4; ++mult) {
            int new_deg = deg + 2;
            int new_temp[10] = {0};
            for (int i = 0; i <= deg; ++i)
                for (int j = 0; j < 3; ++j)
                    new_temp[i + j] += temp[i] * f[j];
            for (int i = 0; i <= new_deg; ++i)
                temp[i] = new_temp[i];
            deg = new_deg;
        }
        /* Multiply by (1 - x) to get Q(x) */
        for (int k = 0; k <= 9; ++k) coeff[k] = 0;
        coeff[0] = temp[0];
        for (int k = 1; k <= 9; ++k)
            coeff[k] = (k <= 8 ? temp[k] : 0) - temp[k - 1];

        /* Polynomial contributions from the numerator N(x) */
        p_term[2] = 4;   /* +4*x^2   */
        p_term[3] = -6;  /* -6*x^3   */
        p_term[4] = 8;   /* +8*x^4   */
        p_term[5] = -3;  /* -3*x^5   */
        init_done = 1;
    }

    /* ------------------------------------------------------------------ */
    /*  Allocate or grow cache as necessary.  cache[0] is unused because
     *  the sequence is defined for n >= 1.                                */
    /* ------------------------------------------------------------------ */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    if (computed_max == 0) {         /* initialise base case */
        cache[0] = (seq_t)0;         /* a(0) is unused but set to 0 */
        if (cache_size > 1) cache[1] = (seq_t)0;
        computed_max = 1;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute missing terms up to n using the recurrence.              */
    /* ------------------------------------------------------------------ */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;  /* contributions with positive sign */
        seq_t neg = (seq_t)0;  /* contributions with negative sign */

        /* Positive contributions: +5*a[i-1] +13*a[i-4] +3*a[i-5] +3*a[i-8] +1*a[i-9] */
        if (i >= 1) pos += cache[i - 1] * (seq_t)5;
        if (i >= 4) pos += cache[i - 4] * (seq_t)13;
        if (i >= 5) pos += cache[i - 5] * (seq_t)3;
        if (i >= 8) pos += cache[i - 8] * (seq_t)3;
        if (i >= 9) pos += cache[i - 9] * (seq_t)1;

        /* Negative contributions: -6*a[i-2] -6*a[i-3] -10*a[i-6] -2*a[i-7] */
        if (i >= 2) neg += cache[i - 2] * (seq_t)6;
        if (i >= 3) neg += cache[i - 3] * (seq_t)6;
        if (i >= 6) neg += cache[i - 6] * (seq_t)10;
        if (i >= 7) neg += cache[i - 7] * (seq_t)2;

        seq_t val = pos;
        val -= neg;                 /* subtract negative part */

        /* Add polynomial terms from numerator (only for i = 2..5) */
        if (i < 10 && p_term[i] != 0) {
            if (p_term[i] > 0)
                val += (seq_t)p_term[i];
            else
                val -= (seq_t)(-p_term[i]);
        }

        cache[i] = val;
    }

    computed_max = n;
    return cache[n];
}
