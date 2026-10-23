/*
 * A159601  E.g.f. S(x) satisfies: S(x) = Integral [1 - 2*S(x)^2]^(3/4) dx
 *          with S(0)=0.
 * https://oeis.org/A159601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159601
   a(n) = -c[2*n] where the coefficients c[k] of the EGF C(x)
   satisfy C^4 + 2S^2 = 1,  C(0)=1,  and S'(x)=C(x)^3.
   The recurrence used is:
        c[0] = 1,   c[1] = 0
        for m ≥ 1:
            conv2_m = Σ_{i=0..m} binom(m,i) * c[i] * c[m-i]
            conv3_m = Σ_{i=0..m} binom(m,i) * c[i] * conv2[m-i]
            c[m+1] = - conv3_{m-1}
   This algorithm runs in O(N²) where N is the maximum required index
   (here 2*n), using only integer arithmetic.  It works for the
   arbitrary‑precision build (seq_t is a big‑int class) and for the
   fixed‑width builds as long as the terms fit within the target type.  */

#include <stdlib.h>
#include <string.h>

/* Static state for memoised computation */
static seq_t *c_coef   = NULL;  /* C coefficients c[k]            */
static seq_t *conv2    = NULL;  /* Coefficients of C(x)^2          */
static seq_t *conv3    = NULL;  /* Coefficients of C(x)^3          */
static size_t arr_cap  = 0;     /* current allocated array size    */
static idx_t  last_m   = 0;     /* largest m for which conv*_m filled */

/* Ensure the arrays have capacity for at least 'need' indices
   (indices 0 .. need inclusive).  New elements are zero‑initialised. */
static void ensure_capacity(idx_t need)
{
    if (arr_cap > (size_t)need) return;

    size_t new_cap = (size_t)need + 4;          /* extra room to reduce reallocs */
    seq_t *new_c   = (seq_t *)realloc(c_coef,   new_cap * sizeof(seq_t));
    seq_t *new_c2  = (seq_t *)realloc(conv2,    new_cap * sizeof(seq_t));
    seq_t *new_c3  = (seq_t *)realloc(conv3,    new_cap * sizeof(seq_t));
    if (!new_c || !new_c2 || !new_c3) abort();

    /* Zero the new part */
    size_t old_cap = arr_cap;
    if (new_c)  memset(new_c  + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    if (new_c2) memset(new_c2 + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    if (new_c3) memset(new_c3 + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));

    c_coef = new_c;
    conv2  = new_c2;
    conv3  = new_c3;
    arr_cap = new_cap;
}

/* Initialise the tables on the first call */
static void init_tables(void)
{
    if (c_coef) return;                     /* already initialised */

    ensure_capacity(1);                     /* at least indices 0,1 */
    c_coef[0] = (seq_t)1;                   /* C(0) = 1 */
    c_coef[1] = (seq_t)0;                   /* C'(0)=0 ⇒ c1 = 0 */

    conv2[0] = (seq_t)1;                    /* (C^2)_0 = 1 */
    conv3[0] = (seq_t)1;                    /* (C^3)_0 = 1 */

    last_m = 0;                             /* conv0 already computed */
}

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* undefined, return 0 */

    init_tables();

    idx_t required_index = 2 * n;           /* need c[2*n] */

    ensure_capacity(required_index + 1);     /* arrays must hold up to c[2*n] */

    /* Incrementally fill conv2, conv3 and c up to the required index */
    while (last_m < required_index)
    {
        idx_t m = last_m + 1; /* next m to process   */

        /* ---------- Compute conv2[m] ---------- */
        seq_t sum2 = (seq_t)0;
        seq_t binom = (seq_t)1; /* binom(m,0) */
        for (idx_t i = 0; i <= m; ++i)
        {
            seq_t term = c_coef[i] * c_coef[m - i];
            sum2 += term * binom;
            if (i < m)
                binom = binom * (m - i) / (i + 1);
        }
        conv2[m] = sum2;

        /* ---------- Compute conv3[m] ---------- */
        seq_t sum3 = (seq_t)0;
        binom = (seq_t)1; /* binom(m,0) */
        for (idx_t i = 0; i <= m; ++i)
        {
            seq_t term = c_coef[i] * conv2[m - i];
            sum3 += term * binom;
            if (i < m)
                binom = binom * (m - i) / (i + 1);
        }
        conv3[m] = sum3;

        /* ---------- Compute next c value ---------- */
        /* c[m+1] = - conv3[m-1] (for m ≥ 1) */
        c_coef[m + 1] = - conv3[m - 1];

        last_m = m;   /* mark m as processed */
    }

    /* a(n) = -c[2*n] */
    return - c_coef[required_index];
}
