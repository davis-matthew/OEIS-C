/*
 * A063333  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          60 ).
 * https://oeis.org/A063333
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the rational generating function
   G(x) = x^2*(7*x^8 + 35*x^7 + 44*x^6 + 37*x^5 + 78*x^4 + 37*x^3 + 36*x^2 + 35*x - 1)
/          ((x - 1)^2*(x + 1)^2*(x^2 - x + 1)*(x^2 + 1))
   The denominator expands to D(x) = x^8 - x^7 + x^5 - 2x^4 + x^3 - x + 1.
   Recurrence: for n >= 0
      a(n) = - Σ_{k=1..8} d_k * a(n-k) + b_n
   where d_k are coefficients of D(x) and b_n is the coefficient of x^n
   in the numerator (zero for n > 10).  The sequence starts with
   a(0)=0, a(1)=0 (values below offset 2 are ignored).  This
   implementation memoises computed values so that calls in any order
   are efficient.  The algorithm runs in O(n) time for the largest
   requested n and uses only integer arithmetic. */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoised values, index i stores a(i) */
static idx_t  capacity   = 0;      /* allocated size of cache (>= computed+1) */
static idx_t  computed   = 0;      /* highest index currently computed */

/* denominator polynomial coefficients D(x) = Σ_{k=0}^8 d_k x^k */
static const seq_t P_coef[9] = {
    1,   /* d0 */
   -1,   /* d1 */
    0,   /* d2 */
    1,   /* d3 */
   -2,   /* d4 */
    1,   /* d5 */
    0,   /* d6 */
   -1,   /* d7 */
    1    /* d8 */
};

/* numerator polynomial coefficients N(x) = Σ_{k=0}^{10} N_k x^k */
static const seq_t N_coef[11] = {
     0,   /* x^0 */
     0,   /* x^1 */
    -1,   /* x^2 */
    35,   /* x^3 */
    36,   /* x^4 */
    37,   /* x^5 */
    78,   /* x^6 */
    37,   /* x^7 */
    44,   /* x^8 */
    35,   /* x^9 */
     7    /* x^10 */
};

/* Ensure cache has room and computed values up to index n */
static void ensure_cache(idx_t n)
{
    /* Allocate initial memory if needed */
    if (cache == NULL) {
        capacity = (n >= 2 ? n + 1 : 3);
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < capacity; ++i)
            cache[i] = 0;            /* values a(0)=a(1)=0 */
        computed = (capacity > 1 ? 1 : 0);
    }

    /* Grow capacity if necessary */
    if (n >= capacity) {
        idx_t new_cap = capacity * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = capacity; i < new_cap; ++i)
            cache[i] = 0;
        capacity = new_cap;
    }

    /* Compute missing terms */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t rhs = (i <= 10) ? N_coef[i] : 0;
            seq_t sum = 0;
            for (int k = 1; k <= 8; ++k) {
                if (i >= (idx_t)k) {
                    sum += P_coef[k] * cache[(size_t)(i - k)];
                }
            }
            cache[(size_t)i] = rhs - sum;
        }
        computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;           /* values below offset 2 are irrelevant */
    ensure_cache(n);
    return cache[(size_t)n];
}
