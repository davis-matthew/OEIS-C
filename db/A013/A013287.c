/*
 * A013287  Expansion of e.g.f. sinh(log(x+1) - tanh(x)).
 * https://oeis.org/A013287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A013287
    Expansion of the exponential generating function
    sinh( log(x+1) - tanh(x) ).
    The function a(idx_t n) returns the integer coefficient a(n)
    in the series F(x) = sum_{n>=0} a(n) * x^n / n!.
    The implementation uses only integer arithmetic and
    a simple recurrence for the e.g.f's coefficients. */

#include <stdlib.h>
#include <stdint.h>

/* External integer types supplied by the build system:
   seq_t – the type for sequence values (int8_t … __int128 or GMP-type)
   idx_t – the type for indices   (uint64_t)              */

/* Build and cache the coefficients up to a given index.
   The cached arrays are indexed from 0 to n.  All arithmetic uses
   seq_t; factorials are stored in fact[].  All other arrays contain
   the sequence values scaled by n! (i.e. coefficient * n!). */
static idx_t cache_max = 0;
static seq_t *fact_arr  = NULL;  /* factorials */
static seq_t *T_arr     = NULL;  /* tanh coefficients scaled by n! */
static seq_t *L_arr     = NULL;  /* log coefficients scaled by n! */
static seq_t *G_arr     = NULL;  /* g(x) = log - tanh, scaled by n! */
static seq_t *Eplus_arr = NULL;  /* exp(G) */
static seq_t *Eminus_arr= NULL;  /* exp(-G) */

/* free any existing cache */
static void free_cache(void)
{
    if (fact_arr) free(fact_arr);
    if (T_arr)    free(T_arr);
    if (L_arr)    free(L_arr);
    if (G_arr)    free(G_arr);
    if (Eplus_arr)free(Eplus_arr);
    if (Eminus_arr)free(Eminus_arr);
    fact_arr = T_arr = L_arr = G_arr = Eplus_arr = Eminus_arr = NULL;
    cache_max = 0;
}

/* Simple fast factorial (n! for n<=cache_max) */
static void build_cache(idx_t n)
{
    if (n <= cache_max) return;     /* already built */

    free_cache();                    /* start fresh for simplicity */

    fact_arr  = (seq_t*)malloc((n+1)*sizeof(seq_t));
    T_arr     = (seq_t*)malloc((n+1)*sizeof(seq_t));
    L_arr     = (seq_t*)malloc((n+1)*sizeof(seq_t));
    G_arr     = (seq_t*)malloc((n+1)*sizeof(seq_t));
    Eplus_arr= (seq_t*)malloc((n+1)*sizeof(seq_t));
    Eminus_arr= (seq_t*)malloc((n+1)*sizeof(seq_t));

    /* factorials */
    fact_arr[0] = 1;
    for (idx_t i=1; i<=n; ++i)
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;

    /* tanh coefficients scaled by n!   (T[0] = 0) */
    T_arr[0] = 0;
    T_arr[1] = 1;   /* t1 = 1  => T[1] = 1! = 1 */
    for (idx_t m=1; m<n; ++m) {
        /* compute s_m_scaled = sum_{k=0}^m binom(m,k) * T[k] * T[m-k] */
        seq_t sum = 0;
        seq_t comb = 1;   /* binom(m,0) */
        for (idx_t k=0; k<=m; ++k) {
            seq_t term = comb * T_arr[k] * T_arr[m-k];
            sum += term;

            /* advance comb to binom(m,k+1) */
            if (k < m) {
                comb = comb * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }
        T_arr[m+1] = -sum;          /* T[n+1] = -s_n_scaled */
    }

    /* log coefficients scaled by n!   (L[0] = 0) */
    L_arr[0] = 0;
    for (idx_t i=1; i<=n; ++i) {
        seq_t sign = ((i & 1) == 0) ? -1 : 1;  /* (-1)^{i-1} */
        L_arr[i] = sign * fact_arr[i-1];
    }

    /* g(x) = log - tanh, scaled by n!  */
    G_arr[0] = 0;
    for (idx_t i=1; i<=n; ++i)
        G_arr[i] = L_arr[i] - T_arr[i];

    /* exp(G)  and exp(-G)  via the standard e.g.f convolution */
    Eplus_arr[0]  = 1;
    Eminus_arr[0] = 1;
    for (idx_t m=1; m<=n; ++m) {
        seq_t sum_plus  = 0;
        seq_t sum_minus = 0;
        seq_t comb_plus  = 1; /* binom(m-1,0) */
        seq_t comb_minus = 1;
        for (idx_t k=1; k<=m; ++k) {
            /* term for exp(G) */
            seq_t t_plus  = comb_plus * G_arr[k] * Eplus_arr[m-k];
            sum_plus += t_plus;

            /* term for exp(-G) : use -G */
            seq_t t_minus = comb_minus * (-G_arr[k]) * Eminus_arr[m-k];
            sum_minus += t_minus;

            /* advance combinations if needed */
            if (k < m) {
                comb_plus  = comb_plus  * (seq_t)(m - k) / (seq_t)k;
                comb_minus = comb_minus * (seq_t)(m - k) / (seq_t)k;
            }
        }
        Eplus_arr[m]  = sum_plus;
        Eminus_arr[m] = sum_minus;
    }

    cache_max = n;
}

/* Return the a(n) coefficient.  The result is a signed integer. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* a(0)=0 */
    build_cache(n);
    seq_t diff = Eplus_arr[n] - Eminus_arr[n];
    return diff / (seq_t)2;        /* divide by 2, exact division */
}
