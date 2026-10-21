/*
 * A228831  Expansion of psi(x)^2 * phi(-x^2)^4 in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A228831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of * (∏_{k>0} (1 - x^(2k))^3 * (1 + x^k) / (1
 *            + x^(2k))^2)^2 * * Recurrence (derived from e_m = exponent of (1
 *            - x^m)): * let e_m = -2 if m odd * = 10 if m ≡ 2 (mod 4) * = 6
 *            if m ≡ 0 (mod 4) * let σ_j = Σ_{d|j} d * e_d * a(0) = 1 * for n
 *            ≥ 1: * a(n) = –( Σ_{j=1..n} a(n‑j) * σ_j...
 * Range:     i8 n <= 4, i16 n <= 114, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage – the build system supplies these types */
static idx_t _max_cached     = 0;
static seq_t * _coeff_cache  = NULL;
static seq_t * _sigma_cache  = NULL;

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* extend cache if necessary */
    if (n > _max_cached) {
        idx_t new_size = n;                 /* we need indices 0..n */
        size_t alloc_size = (size_t)new_size + 1;

        /* allocate zero‑initialised storage (explicit cast for C++ ) */
        seq_t *new_coeff = (seq_t*)calloc(alloc_size, sizeof(seq_t));
        seq_t *new_sigma = (seq_t*)calloc(alloc_size, sizeof(seq_t));

        /* Compute σ_j = Σ_{d|j} d * e_d  for j = 1..new_size */
        for (idx_t d = 1; d <= new_size; ++d) {
            seq_t e;
            if (d & 1)           e = (seq_t)(-2);
            else if ((d & 3) == 0) e = (seq_t)6;
            else                 e = (seq_t)10;

            seq_t prod = (seq_t)d * e;
            for (idx_t m = d; m <= new_size; m += d)
                new_sigma[m] += prod;
        }

        /* Compute coefficients a[n] using the recurrence */
        new_coeff[0] = (seq_t)1;         /* a(0) = 1 */
        for (idx_t i = 1; i <= new_size; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= i; ++j)
                sum += new_coeff[i - j] * new_sigma[j];
            new_coeff[i] = -(sum / (seq_t)i);
        }

        /* replace old cached data (free may handle NULL) */
        free(_coeff_cache);
        free(_sigma_cache);
        _coeff_cache = new_coeff;
        _sigma_cache = new_sigma;
        _max_cached  = new_size;
    }

    return _coeff_cache[n];
}
