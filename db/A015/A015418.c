/*
 * A015418  Gaussian binomial coefficient [ n,11 ] for q=-11.
 * https://oeis.org/A015418
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 11, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A015418
 * Formula: a(n) = Product_{i=1..11} ((-11)^(n-i+1)-1) / ((-11)^i-1)
 * Offset: 11  (a(11) = 1)
 */

#include <stdlib.h>

#define K 11  /* fixed upper index of the Gaussian binomial */

static idx_t max_cached_n = 0;   /* highest n for which cache is filled */
static seq_t *cache      = NULL; /* cache[n] holds a(n) for n >= 11 */
static size_t cache_len  = 0;     /* allocated size of cache */

static seq_t *coeff_prev = NULL; /* previous row of coefficients */
static seq_t *coeff_curr = NULL; /* current row of coefficients */
static seq_t *pow_q      = NULL; /* powers of q = -11: pow_q[e] = (-11)^e */
static idx_t pow_q_len   = 0;    /* number of computed powers of q */

/*----- Utility helpers -------------------------------------------------*/
static void ensure_coeff_arrays(void)
{
    if (!coeff_prev) {
        coeff_prev = (seq_t *)calloc(K + 1, sizeof(seq_t));
        coeff_curr = (seq_t *)calloc(K + 1, sizeof(seq_t));
    }
}

static void ensure_pow_q(idx_t needed)
{
    if (!pow_q) {
        pow_q      = (seq_t *)calloc(1, sizeof(seq_t));
        pow_q[0]   = (seq_t)1;
        pow_q_len  = 1;
    }
    while (pow_q_len <= needed) {
        seq_t next = pow_q[pow_q_len - 1] * (seq_t)(-11);
        pow_q = (seq_t *)realloc(pow_q, (pow_q_len + 1) * sizeof(seq_t));
        pow_q[pow_q_len] = next;
        pow_q_len++;
    }
}

static void ensure_cache_size(size_t new_len)
{
    if (cache_len < new_len) {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;
        cache_len = new_len;
    }
}

/*----- Main function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* For indices below the offset the behaviour is unspecified */
    if (n < 11) return (seq_t)0;

    /* Return cached value if already computed */
    if (n <= max_cached_n)
        return cache[n];

    /* Prepare helper structures */
    ensure_coeff_arrays();
    ensure_pow_q(n - 1);          /* we need powers up to (n-1) */
    ensure_cache_size(n + 1);     /* cache indices 0 .. n (unused <11) */

    /* Initialise first row of coefficients for n = 0 */
    for (idx_t j = 0; j <= K; ++j)
        coeff_prev[j] = (seq_t)0;
    coeff_prev[0] = (seq_t)1;

    /* Compute rows up to required n */
    for (idx_t i = 1; i <= n; ++i) {
        coeff_curr[0] = (seq_t)1;
        for (idx_t j = 1; j <= K; ++j) {
            seq_t val = coeff_prev[j];              /* C(i-1,j) */
            if (i >= j) {
                seq_t mul = pow_q[i - j] * coeff_prev[j - 1]; /* q^(i-j)*C(i-1,j-1) */
                val = val + mul;
            }
            coeff_curr[j] = val;
        }
        /* swap rows */
        seq_t *tmp = coeff_prev;
        coeff_prev = coeff_curr;
        coeff_curr = tmp;

        /* cache the desired coefficient if i >= 11 */
        if (i >= 11)
            cache[i] = coeff_prev[11];
    }

    max_cached_n = n;
    return cache[n];
}
