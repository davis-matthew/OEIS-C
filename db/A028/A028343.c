/*
 * A028343  Expansion of Product_{i>=1} (1-x^i)^(1/i); also of exp(-
 *          Sum_{n>=1}(d(n)*x^n/n)) where d(n) is the number of divisors of n.
 * https://oeis.org/A028343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028343: a(n) = n! * coeff_{x^n} exp(- Σ_{k≥1} d(k) x^k / k)
   Recurrence derived from the exponential generating function:
   a(0) = 1,
   for n ≥ 1:
   a(n) = - Σ_{k=1}^{n} d(k) * ( (n-1)! / (n-k)! ) * a(n-k)
*/

#include <stdlib.h>  /* malloc, realloc, abort */

/*--- global caches ------------------------------------------------*/
static unsigned *d_cache = NULL;   /* divisor counts                                  */
static idx_t      d_cap_len = 0;    /* current capacity of d_cache                    */

static seq_t *a_cache = NULL;      /* cache for a(n)                                  */
static idx_t  a_cap_len = 0;       /* capacity of a_cache                             */
static idx_t  a_comp_len = 0;       /* number of a values already computed             */

/*--- accessor helpers ---------------------------------------------*/
static inline seq_t get_a(idx_t m)   { return a_cache[m]; }
static inline void  set_a(idx_t m, seq_t v) { a_cache[m] = v; }

/*--- ensure divisor cache up to n ---------------------------------*/
static void ensure_d_capacity(idx_t n)
{
    if (d_cap_len <= n) {
        idx_t new_cap = d_cap_len ? d_cap_len * 2 : 1;
        while (new_cap <= n) new_cap *= 2;

        unsigned *new_d = (unsigned *)realloc(d_cache, new_cap * sizeof(unsigned));
        if (!new_d) abort();
        d_cache = new_d;

        /* initialise all entries to zero */
        for (idx_t i = 0; i < new_cap; ++i) d_cache[i] = 0;

        /* compute divisor counts for 1..new_cap */
        for (idx_t i = 1; i <= new_cap; ++i)
            for (idx_t j = i; j <= new_cap; j += i)
                ++d_cache[j];

        d_cap_len = new_cap;
    }
}

/*--- ensure a_cache up to n ---------------------------------------*/
static void ensure_a_capacity(idx_t n)
{
    if (a_cap_len <= n) {
        idx_t new_cap = a_cap_len ? a_cap_len * 2 : 1;
        while (new_cap <= n) new_cap *= 2;

        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_a) abort();
        a_cache = new_a;
        a_cap_len = new_cap;
    }
}

/*--- main sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_a_capacity(n);
    ensure_d_capacity(n);

    /* initialise a(0) on first use */
    if (a_comp_len == 0) {
        set_a(0, (seq_t)1);
        a_comp_len = 1;
    }

    /* compute missing a-values up to n */
    for (idx_t m = a_comp_len; m <= n; ++m) {
        seq_t sum   = 0;
        seq_t factor = 1;          /* for k = 1 */

        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1) {
                /* factor = (n-1)! / (n-k)!   for current m */
                factor = factor * (seq_t)(m - k + 1);
            }
            seq_t term = (seq_t)d_cache[k] * factor * get_a(m - k);
            sum += term;
        }
        set_a(m, -sum);
    }
    a_comp_len = n + 1;

    return get_a(n);
}
