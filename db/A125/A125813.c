/*
 * A125813  q-Bell numbers for q=3; eigensequence of A022167, which is the
 *          triangle of Gaussian binomial coefficients [n,k] for q=3.
 * https://oeis.org/A125813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(n) = Σ_{k=0}^{n-1} [n-1 choose k]_3 * a(k) (n > 0)
 *            * [n choose k]_3 is the Gaussian binomial coefficient for q = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- Internal state ---------- */
static seq_t *a_cache      = NULL;  /* memoised a(n) values   */
static size_t a_cache_size = 0;     /* capacity of a_cache    */
static idx_t   a_max_computed = 0;  /* highest index computed */

static seq_t *pow3          = NULL;  /* powers of 3: pow3[i] = 3^i */
static size_t pow3_size     = 0;     /* capacity of pow3       */

/* ---------- Helper: resize a_cache ---------- */
static void ensure_a_cache(size_t needed)
{
    if (a_cache_size <= needed) {
        size_t new_size = a_cache_size ? a_cache_size : 1;
        while (new_size <= needed) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        a_cache = tmp;
        a_cache_size = new_size;
    }
    if (a_max_computed == 0) {          /* initialise the first element */
        a_cache[0] = (seq_t)1;
        a_max_computed = 0;
    }
}

/* ---------- Helper: resize pow3 ---------- */
static void ensure_pow3(size_t needed)
{
    if (pow3_size == 0) {                /* first element 3^0 = 1 */
        pow3 = (seq_t *)malloc(sizeof(seq_t));
        pow3[0] = (seq_t)1;
        pow3_size = 1;
    }
    if (pow3_size <= needed) {
        size_t new_size = pow3_size;
        while (new_size <= needed) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow3, new_size * sizeof(seq_t));
        for (size_t i = pow3_size; i < new_size; ++i)
            tmp[i] = tmp[i - 1] * (seq_t)3;
        pow3 = tmp;
        pow3_size = new_size;
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* base case */

    /* Ensure the cache can hold up to index n */
    ensure_a_cache((size_t)n);

    /* Compute all terms up to n that are not yet computed */
    if (a_max_computed < n) {
        for (idx_t idx = a_max_computed + 1; idx <= n; ++idx) {
            idx_t m = idx - 1;                     /* m = n-1 for this step */

            /* Need powers of 3 up to m + 1 (to compute numerator) */
            ensure_pow3((size_t)(m + 1));

            seq_t sum = a_cache[0];                /* k = 0 term, [m,0] = 1 */
            seq_t row = (seq_t)1;                  /* current Gaussian binomial C(m,k) */

            /* k = 1 .. m */
            for (size_t k = 1; k <= (size_t)m; ++k) {
                seq_t num = pow3[(size_t)m - k + 1] - (seq_t)1;
                seq_t den = pow3[k] - (seq_t)1;
                row = (row * num) / den;           /* C(m,k) from C(m,k-1) */
                sum += row * a_cache[(idx_t)k];
            }

            a_cache[(size_t)idx] = sum;
        }
        a_max_computed = n;
    }

    return a_cache[(size_t)n];
}
