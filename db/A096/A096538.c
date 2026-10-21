/*
 * A096538  E.g.f.: A(x) =
 *          exp(x*exp(2*x*exp(2^2*x*exp(...exp(2^n*x*exp(...))...)))), for
 *          n>=0.
 * https://oeis.org/A096538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from OEIS:
   a(0) = 1
   a(n+1) = Σ_{i=0..n} (i+1)·2^i·C(n,i)·a(i)·a(n-i)   for n ≥ 0   */
#include <stdlib.h>

/*--- cache for computed a(n) values ----------------------------------*/
static seq_t *a_cache      = NULL;   /* a_cache[n] == a(n) */
static size_t a_cache_len  = 0;      /* allocated size of a_cache   */
static idx_t  a_computed   = 0;      /* highest n for which a(n) is known */

/*--- cache for powers of two ------------------------------------------*/
static seq_t *pow2_cache   = NULL;   /* pow2_cache[i] == 2^i          */
static size_t pow2_len     = 0;       /* allocated size of pow2_cache  */

/*--- ensure a_cache can hold indices up to n -------------------------*/
static void ensure_a_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (a_cache_len < need) {
        size_t newlen = a_cache_len ? a_cache_len : 4;
        while (newlen < need) newlen <<= 1;
        a_cache = (seq_t *)realloc(a_cache, newlen * sizeof(seq_t));
        a_cache_len = newlen;
    }
    if (a_computed == 0) {          /* first time: set a(0)=1 */
        a_cache[0] = (seq_t)1;
        a_computed = 0;
    }
}

/*--- ensure pow2_cache can hold 2^i for i ≤ n ------------------------*/
static void ensure_pow2_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (pow2_len < need) {
        size_t newlen = pow2_len ? pow2_len : 4;
        while (newlen < need) newlen <<= 1;
        pow2_cache = (seq_t *)realloc(pow2_cache, newlen * sizeof(seq_t));
        /* initialise new entries */
        size_t start = pow2_len;
        if (start == 0) {            /* first entry must be 1 */
            pow2_cache[0] = (seq_t)1;
            start = 1;
        }
        for (size_t i = start; i < newlen; ++i)
            pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
        pow2_len = newlen;
    }
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) defined explicitly */

    ensure_a_cache(n);
    ensure_pow2_cache(n-1);          /* need powers up to 2^(n-1) */

    /* compute all a(k) for k > a_computed up to n */
    for (idx_t k = a_computed + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;       /* C(k-1,0) */

        for (idx_t i = 0; i < k; ++i) {
            /* product: (i+1)*2^i*binom*a(i)*a(k-1-i) */
            seq_t prod = (seq_t)(i+1);
            prod *= pow2_cache[i];
            prod *= binom;
            prod *= a_cache[i];
            prod *= a_cache[k-1-i];

            sum += prod;

            /* next binomial coefficient: C(k-1, i+1) */
            if (i + 1 < k) {
                binom = binom * (seq_t)(k-1-i) / (seq_t)(i+1);
            }
        }
        a_cache[k] = sum;
    }
    a_computed = n > a_computed ? n : a_computed;
    return a_cache[n];
}
