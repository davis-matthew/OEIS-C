/*
 * A214316  Expansion of psi(x)^2 - 5 * x * psi(x^5)^2 in powers of x where
 *          psi() is a Ramanujan theta function.
 * https://oeis.org/A214316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 185, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A214316
    The sequence is given by  
        a(n) = f(n) – 5·f((n-1)/5)   if (n-1) mod 5 = 0  
               f(n)                  otherwise  
    where f(k) is the number of ordered pairs of non‑negative integers
    (i, j) with triangular numbers T(i)+T(j) = k,  T(i)=i(i+1)/2.
    f(0)=1.  f(k) is computed by a double loop over the triangular
    numbers not exceeding k.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *f_cache = NULL;   /* cache of f(k) for k in [0, f_limit-1] */
static idx_t f_limit = 0;       /* current cache size */

/* Compute f(k) for k = 0..max_n and store in f[0..max_n]        */
static void
compute_f(seq_t *f, idx_t max_n)
{
    /* initialise */
    for (idx_t i = 0; i <= max_n; ++i) {
        f[i] = 0;
    }

    /* count ordered pairs of triangular numbers */
    for (idx_t i = 0; ; ++i) {
        seq_t Ti = (seq_t)i * ((seq_t)i + 1) / 2;
        if (Ti > (seq_t)max_n) break;

        for (idx_t j = 0; ; ++j) {
            seq_t Tj = (seq_t)j * ((seq_t)j + 1) / 2;
            seq_t sum = Ti + Tj;
            if (sum > max_n) break;

            f[(idx_t)sum] += 1;
        }
    }
}

seq_t a(idx_t n)
{
    /* grow cache if necessary */
    if (n + 1 > f_limit) {
        idx_t new_lim = n + 1;
        f_cache = (seq_t *)realloc(f_cache, sizeof(seq_t) * new_lim);
        compute_f(f_cache, n);
        f_limit = new_lim;
    }

    /* a(n) = f(n) – 5·f((n-1)/5) when (n-1) ≡ 0 (mod 5) */
    seq_t result = f_cache[n];
    if (n >= 1U && ((n - 1) % 5) == 0) {
        idx_t k = (n - 1) / 5;
        result -= (seq_t)5 * f_cache[k];
    }
    return result;
}
