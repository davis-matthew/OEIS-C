/*
 * A013182  exp(arctanh(x)+sinh(x))=1+2*x+4/2!*x^2+11/3!*x^3+40/4!*x^4+177/5!*x^5...
 * https://oeis.org/A013182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} C(n-1,k-1) * b(k) * a(n-k), where a(0)=1 and
 *            b(k) = (k-1)! + 1 if k is odd, 0 otherwise. Since
 *            C(n-1,k-1)*(k-1)! = (n-1)!/(n-k)!, we can compute the summand as
 *            ( (n-1)!/(n-k)! + C(n-1,k-1) ) * a(n-k) with no factorial table
 *            required
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;          /* a[0] … a[max] */
static size_t a_cache_size = 0;        /* allocated size of a_cache */
static idx_t a_computed_upto = (idx_t)-1;   /* highest n with a[n] computed */

static void ensure_sequence(idx_t n)
{
    /* Resize cache if necessary */
    if (a_cache == NULL || a_cache_size <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_a = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (a_cache) {
            for (size_t i = 0; i < a_cache_size; ++i)
                new_a[i] = a_cache[i];
            free(a_cache);
        }
        a_cache = new_a;
        a_cache_size = new_size;
    }

    /* Compute missing a[n] values */
    for (idx_t m = a_computed_upto + 1; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(m-1,0) */
        seq_t val   = (seq_t)1;      /* (m-1)!/(m-1)! = 1 */

        for (idx_t k = 1; k <= m; ++k) {
            if (k & 1U) {        /* k odd, use b(k) */
                seq_t term = binom + val;          /* binom + val = binom*((k-1)!+1) */
                term *= a_cache[m - k];
                sum += term;
            }

            if (k < m) {
                /* Update binomial coefficient C(m-1,k) from C(m-1,k-1) */
                binom = (binom * (seq_t)(m - k)) / (seq_t)k;
                /* Update (m-1)!/(m-k)! from previous value */
                val *= (seq_t)(m - k);
            }
        }

        a_cache[m] = sum;
    }
    a_computed_upto = n;
}

seq_t a(idx_t n)
{
    /* For safety, guard against underflow; not expected in normal use */
    if (n == (idx_t)-1) return (seq_t)0;

    ensure_sequence(n);
    return a_cache[n];
}
