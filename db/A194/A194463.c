/*
 * A194463  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) < 0, where
 *          r=(1+sqrt(5))/2 and c=(1+sqrt(5))/4, and < > denotes fractional
 *          part.
 * https://oeis.org/A194463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers m such that * Sum_{k=1..m} ( ⟨c + k*r⟩ – ⟨k*r⟩ ) < 0, *
 *            where r = (1+√5)/2 and c = (1+√5)/4
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Auxiliary constants */
static const long double SQRT5  = sqrtl(5.0L);
static const long double PHI    = (1.0L + SQRT5) / 2.0L;   /* r   */
static const long double T_VAL  = (3.0L - SQRT5) / 4.0L;   /* 1 – c */
static const long double EPS    = 1e-18L;

/*  Cache of computed terms */
static size_t  terms_cap   = 0;
static size_t  terms_len   = 0;
static seq_t*  terms       = NULL;
static idx_t   processed_m = 0;   /* last m processed globally   */
static idx_t   a_small     = 0;   /* number of k <= m with {k*PHI} < T_VAL */

/*  Ensure space for at least n terms    */
static void ensure_capacity(size_t n)
{
    if (terms_cap >= n) return;
    size_t newcap = terms_cap ? terms_cap << 1 : 64;
    while (newcap < n) newcap <<= 1;
    terms = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
    terms_cap = newcap;
}

/*  Main function required by the test harness   */
seq_t a(idx_t n)
{
    /*  Grow cache if we don't yet have n terms  */
    while (terms_len < n)
    {
        /*  Compute next m  */
        ++processed_m;

        /*  Fractional part of m*PHI  */
        long double val  = (long double)processed_m * PHI;
        long double frac = val - floorl(val);

        /*  Increment a_small if fractional part < T_VAL  */
        if (frac + EPS < T_VAL)
            ++a_small;

        /*  floor( m * T_VAL )  */
        idx_t floor_mt = (idx_t)floorl((long double)processed_m * T_VAL);

        /*  Condition for this m  */
        if (a_small <= floor_mt)
        {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)processed_m;
        }
    }

    return terms[n-1];
}
