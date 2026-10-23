/*
 * A186185  Expansion of 1/(1 - x*A001764(x/(1-x))/(1-x)).
 * https://oeis.org/A186185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186185
   a(n) = Sum_{m=1}^{n} Sum_{k=m}^{n} binomial(n-1,k-1) * m/(2*k-m) *
          binomial(3*k-2*m-1,k-m),  n>0,  a(0)=1.
   The recurrence used below (valid for n>6) is
        2*(2*n-1)*(n-1)*a(n) +
        (9*n^2-127*n+170)*a(n-1) +
        10*(-57*n^2+368*n-559)*a(n-2) +
        (2579*n^2-18451*n+32892)*a(n-3) +
        3*(-1633*n^2+13195*n-26878)*a(n-4) +
        6*(712*n-3111)*(n-5)*a(n-5) -
        1395*(n-5)*(n-6)*a(n-6) = 0
   Initial terms (n=0..6) are supplied explicitly.  The function
   memoises results so that subsequent calls for larger n reuse
   previously computed values.  No external libraries are required
   beyond those already supplied by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The integer types seq_t and idx_t are defined by the build. */
/* static seq_t a(idx_t n) is declared elsewhere; here we provide the definition. */

/* Initial values a(0)..a(6).  They are copied into the cache on first use. */
static const seq_t init7[7] = {
    (seq_t)1,      /* a(0) */
    (seq_t)1,      /* a(1) */
    (seq_t)3,      /* a(2) */
    (seq_t)11,     /* a(3) */
    (seq_t)48,     /* a(4) */
    (seq_t)239,    /* a(5) */
    (seq_t)1306    /* a(6) */
};

/* Cached values of the sequence, initially NULL. */
static seq_t *cache = NULL;
static idx_t  cached_cap = 0;     /* allocated capacity */
static idx_t  max_computed = 6;   /* highest n whose value is stored */

/* Make sure the cache can hold at least n+1 values and copy the initial
   seven values if the cache is being created for the first time. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate space for at least n+1 elements, but keep at least 7 */
        cached_cap = (n >= 7) ? (n + 1) : 7;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        for (idx_t i = 0; i < 7; ++i) cache[i] = init7[i];
    } else if (n + 1 > cached_cap) {
        idx_t new_cap = cached_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cached_cap = new_cap;
    }
}

/* Main sequence function.  It is thread‑unsafe due to static storage
   but satisfies the required calling semantics. */
seq_t a(idx_t n)
{
    /* Return pre‑computed values immediately. */
    if (n < 7) return init7[n];

    /* Make sure we have room for the requested index. */
    ensure_cache(n);

    /* If the value is already computed, return it. */
    if (n <= max_computed) return cache[n];

    /* Compute all intermediate values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Pull the previous six values. */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        /* Coefficients as signed 64‑bit integers. */
        int64_t nn      = (int64_t)i;
        int64_t coeff1  = 9*nn*nn - 127*nn + 170;
        int64_t coeff2  = 10 * (-57*nn*nn + 368*nn - 559);
        int64_t coeff3  = 2579*nn*nn - 18451*nn + 32892;
        int64_t coeff4  = 3 * (-1633*nn*nn + 13195*nn - 26878);
        int64_t coeff5  = 6 * (712*nn - 3111) * (nn - 5);
        int64_t coeff6  = -1395 * (nn - 5) * (nn - 6);  /* note the minus sign */

        /* Separate positive and negative contributions to the
           numerator of a(i) = -(coeff1*a1+...+coeff6*a6) / denom. */
        seq_t sum_pos = 0;   /* contributions that become positive   */
        seq_t sum_neg = 0;   /* contributions that become negative  */

        /* Helper macro to add a single term. */
        #define ADD_TERM(A, COEFF)                                   \
            do {                                                    \
                int64_t c = (COEFF);                                \
                seq_t abs_c = (c >= 0) ? (seq_t)c : (seq_t)-c;      \
                seq_t term = (A) * abs_c;                          \
                if (c >= 0)   sum_neg += term; /* -coeff*a -> negative */ \
                else          sum_pos += term; /* -(-coeff)*a -> positive */ \
            } while (0)

        ADD_TERM(a1, coeff1);
        ADD_TERM(a2, coeff2);
        ADD_TERM(a3, coeff3);
        ADD_TERM(a4, coeff4);
        ADD_TERM(a5, coeff5);
        ADD_TERM(a6, coeff6);

        #undef ADD_TERM

        /* The numerator is sum_pos - sum_neg (guaranteed non‑negative). */
        seq_t numerator = sum_pos - sum_neg;

        /* Denominator = 2*(2*n-1)*(n-1), which is always positive for n>6. */
        seq_t denom = (seq_t)2 * ((seq_t)2 * (seq_t)i - (seq_t)1) * ((seq_t)i - (seq_t)1);

        /* Compute the current term and store it. */
        cache[i] = numerator / denom;
    }

    max_computed = n;
    return cache[n];
}
