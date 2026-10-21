/*
 * A061688  Generalized Bell numbers: column 7 of A275043.
 * https://oeis.org/A061688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1..n} a(n-m) * C(n-1, m-1) * (C(n, m))^6 with a(0) =
 *            1 (C(n, k) is the binomial coefficient). This follows from the
 *            exponential generating function Σ a(n) x^n/(n!)^7 = exp( Σ
 *            x^k/(k!)^7 ). The recurrence is derived by the standard rule for
 *            exponentials in exponential generating functions
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static idx_t _max_computed = (idx_t)(-1);   /* largest n computed so far */
static seq_t * _cache      = NULL;
static size_t _cache_cap   = 0;

/* Ensure the cache can hold indices up to n.
   Allocate tiered blocks of power‑2 sizes. */
static void _ensure_cache(idx_t n)
{
    if (_cache == NULL) {
        /* first allocation */
        _cache_cap = (size_t)(n + 1 > 16 ? n + 1 : 16);
        _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
        if (_cache == NULL) abort();
        _cache[0] = (seq_t)1;          /* a(0) = 1 */
        _max_computed = 0;
    } else if ((size_t)n >= _cache_cap) {
        size_t new_cap = _cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        _cache = tmp;
        _cache_cap = new_cap;
    }
}

/* Compute a(n) for arbitrary n (n ≥ 0). */
seq_t a(idx_t n)
{
    _ensure_cache(n);

    /* Compute missing values up to n */
    for (idx_t curr = _max_computed + 1; curr <= n; ++curr) {
        seq_t sum = (seq_t)0;

        seq_t binom_n1_m1 = (seq_t)1;           /* C(curr-1, 0)   */
        seq_t binom_nm   = (seq_t)curr;         /* C(curr, 1)     */

        /* Loop over m = 1..curr */
        for (idx_t m = 1; m <= curr; ++m) {
            /* term = a(curr-m) * C(curr-1, m-1) * (C(curr, m))^6 */
            seq_t term = _cache[curr - m];
            term = term * binom_n1_m1;

            /* Compute (C(curr, m))^6 by repeated multiplication */
            seq_t pow6 = binom_nm;
            for (int k = 1; k < 6; ++k) pow6 *= binom_nm;
            term = term * pow6;

            sum += term;

            if (m == curr) break;     /* avoid updating after last term */

            /* Update binomial coefficients for next m */
            /* C(curr-1, m) = C(curr-1, m-1) * (curr-m) / m */
            seq_t numerator   = (seq_t)(curr - m);
            seq_t denominator = (seq_t)m;
            binom_n1_m1 = (binom_n1_m1 * numerator) / denominator;

            /* C(curr, m+1) = C(curr, m) * (curr-m) / (m+1) */
            denominator = (seq_t)(m + 1);
            binom_nm = (binom_nm * numerator) / denominator;
        }

        _cache[curr] = sum;
    }

    _max_computed = n;
    return _cache[n];
}
