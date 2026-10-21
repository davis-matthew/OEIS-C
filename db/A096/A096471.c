/*
 * A096471  Number of degree-n permutations with exactly one even cycle.
 * https://oeis.org/A096471
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of permutations of n with exactly one even cycle.
 *            Recurrence (derivation from OEIS entry): a(2)=1 , a(3)=3 ,
 *            a(4)=12 , a(5)=60 For n≥6: a(n) = 3*a(n-1) -
 *            (-2*n²+8*n-3)*a(n-2) - (n-2)*(4*n-15)*a(n-3) -
 *            (n-3)*(n³-9*n²+22*n-7)*a(n-4) + (n-3)*(n-4)*(n-5)²*a(n-5)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

static seq_t *a_cache   = NULL;      /* array holding computed values */
static size_t  a_cap    = 0;          /* current capacity of a_cache   */
static idx_t   a_max    = 0;          /* highest index computed so far */

/* enlarge a_cache so that indices up to needed are available */
static void ensure_a(idx_t needed)
{
    if (a_cap > (size_t)needed)
        return;

    size_t new_cap = (size_t)needed + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */

    /* zero-initialize the newly allocated part */
    if (a_cap < new_cap) {
        memset(tmp + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
    }
    a_cache = tmp;
    a_cap   = new_cap;
}

/* compute values up to n (if they are not already computed) */
static void compute_a_upto(idx_t n)
{
    if (n <= a_max)
        return;
    if (n < 2)
        return;                       /* values below offset are irrelevant */

    ensure_a(n);

    /* initialise the base cases once */
    if (a_max < 5) {
        if (a_cap < 6)
            ensure_a(5);
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)1;
        a_cache[3] = (seq_t)3;
        a_cache[4] = (seq_t)12;
        a_cache[5] = (seq_t)60;
        a_max = 5;
    }

    /* compute subsequent terms in ascending order */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        if (i <= 5) {
            /* base values already set; skip */
            continue;
        }

        /* term1 = 3*a[i-1] */
        seq_t term = a_cache[i-1] * (seq_t)3;

        /* coeff2 = -2*i*i + 8*i - 3  (may be negative) */
        long long coeff2 = -2LL*(long long)i*(long long)i + 8LL*(long long)i - 3LL;
        long long abs_c2 = coeff2 >= 0 ? coeff2 : -coeff2;
        seq_t t2 = a_cache[i-2] * (seq_t)abs_c2;
        term = (coeff2 >= 0) ? term - t2 : term + t2;

        /* coeff3 = (i-2)*(4*i-15)  (positive for i≥6) */
        long long coeff3 = (long long)(i-2) * ((long long)4*i - 15LL);
        seq_t t3 = a_cache[i-3] * (seq_t)coeff3;
        term -= t3;

        /* coeff4 = (i-3)*(i^3-9*i^2+22*i-7)  (positive for i≥6) */
        long long coeff4 = (long long)(i-3) *
                           ((long long)i*(long long)i*(long long)i
                            - 9LL*(long long)i*(long long)i
                            + 22LL*(long long)i
                            - 7LL);
        seq_t t4 = a_cache[i-4] * (seq_t)coeff4;
        term -= t4;

        /* coeff5 = -(i-3)*(i-4)*(i-5)^2  (negative for i≥6) */
        long long abs_c5 = (long long)(i-3) *
                           (long long)(i-4) *
                           (long long)(i-5) *
                           (long long)(i-5);   /* absolute value */
        seq_t t5 = a_cache[i-5] * (seq_t)abs_c5;
        term += t5;

        a_cache[i] = term;
    }
    a_max = n;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    compute_a_upto(n);
    return a_cache[n];
}
