/*
 * A084805  Least positive integers, all distinct, that satisfy Sum_{n>0}
 *          1/a(n)^z = 0, where z=(1+I)/sqrt(2).
 * https://oeis.org/A084805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/*  
 * A084805 – least positive integers (all distinct) such that
 * the partial sums S_k = sum_{i=1}^k 1/a(i)^z with z=(1+i)/√2
 * have monotonically decreasing modulus.
 *
 * The algorithm keeps the complex partial sum S_k in double‑precision
 * long double and greedily adds the smallest unused integer m that
 * does not increase |S_k|.  Because the sequence is strictly increasing,
 * a simple linear scan of integers suffices – no usage table is needed.
 * 
 * Formula used:
 *   v(m) = m^{-1/√2} * ( cos( ln m / √2 ) - i sin( ln m / √2 ) )
 *   add v(m) to S_k if |S_k + v(m)| ≤ |S_k|.
 */

/* constants for the exponent needed in v(m) */
static const long double INV_SQRT2 = 1.0L / sqrtl(2.0L);
static const long double EPS      = 1e-20L;  /* tolerance for comparison */

/* static state for memoisation */
static seq_t  *terms       = NULL;
static size_t  terms_cap   = 0;
static size_t  terms_size  = 0;
static long double sum_re = 0.0L, sum_im = 0.0L, cur_mag2 = 0.0L;
static uint64_t next_candidate = 2ULL;

/* utility to enlarge the terms array */
static void grow_terms(void)
{
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* main function requested by the build system */
seq_t
a(idx_t n)
{
    /* adjust for 1‑based index of OEIS */
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */

    /* initialise on first run or after restart */
    if (terms_size == 0) {
        grow_terms();
        terms[0] = (seq_t)1;       /* a(1) = 1 */
        terms_size = 1;
        sum_re   = 1.0L;           /* 1^(-z) = 1 */
        sum_im   = 0.0L;
        cur_mag2 = 1.0L;           /* |S_1|^2 */
        next_candidate = 2ULL;
    }

    /* compute terms up to requested n */
    while (terms_size < (size_t)n) {
        /* search for next admissible integer */
        while (true) {
            /* compute v(next_candidate) */
            long double m_ld = (long double)next_candidate;
            long double r = expl(-logl(m_ld) * INV_SQRT2);        /* m^{-1/√2}  */
            long double theta = logl(m_ld) * INV_SQRT2;           /* ln m / √2   */
            long double v_re =  r * cosl(theta);
            long double v_im = -r * sinl(theta);                 /* negative sign for e^{-iθ} */

            long double new_re = sum_re + v_re;
            long double new_im = sum_im + v_im;
            long double new_mag2 = new_re * new_re + new_im * new_im;

            if (new_mag2 <= cur_mag2 + EPS) {
                /* accept this integer */
                if (terms_size == terms_cap) grow_terms();
                terms[terms_size++] = (seq_t)next_candidate;
                sum_re = new_re;
                sum_im = new_im;
                cur_mag2 = new_mag2;
                next_candidate++;
                break;                     /* move to next term */
            }
            /* otherwise try next integer */
            next_candidate++;
        }
    }

    return terms[n - 1];
}
