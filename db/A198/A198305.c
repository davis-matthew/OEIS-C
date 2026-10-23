/*
 * A198305  L.g.f.: Sum_{n>=1} (x^n/n) / Product_{d|n} (1 - n*x^d/d).
 * https://oeis.org/A198305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * coefficient of x^n in L(x) where L(x) = sum_{k>=1}
 *            (x^k/k) / product_{d|k} (1 - k*x^d/d)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build system */
extern seq_t a(idx_t n);

/* Internal static storage for memoisation */
static idx_t _computed_max = 0;
static seq_t *_A = NULL;          /* array of a[1..N]   */
static seq_t *_poly = NULL;       /* temporary poly array */
static size_t _poly_size = 0;

/* Ensure the temporary poly array is large enough for index N */
static void _ensure_poly(size_t N)
{
    if (_poly_size <= N) {
        free(_poly);
        _poly = (seq_t*)malloc((N + 1) * sizeof(seq_t));
        _poly_size = N + 1;
    }
}

/* Compute all terms a[1..N] from scratch */
static void _compute_to(idx_t N)
{
    if (_computed_max >= N) return;          /* already computed */

    /* Allocate and initialise result array */
    if (_A) free(_A);
    _A = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= N; ++i) _A[i] = 0;

    /* Allocate temporary array */
    _ensure_poly(N);

    for (idx_t t = 1; t <= N; ++t) {
        idx_t m_max = N - t;                /* largest offset for this t */

        /* initialise poly[0] and zero the rest up to m_max */
        _poly[0] = 1;
        for (idx_t i = 1; i <= m_max; ++i) _poly[i] = 0;

        /* enumerate divisors of t */
        for (idx_t d = 1; d * d <= t; ++d) {
            if (t % d == 0) {
                /* first divisor d */
                seq_t factor = (seq_t)(t / d);
                for (idx_t m = d; m <= m_max; ++m)
                    _poly[m] += factor * _poly[m - d];

                /* second divisor t/d, if distinct */
                idx_t d2 = t / d;
                if (d2 != d) {
                    seq_t factor2 = (seq_t)(t / d2);   /* equals d */
                    for (idx_t m = d2; m <= m_max; ++m)
                        _poly[m] += factor2 * _poly[m - d2];
                }
            }
        }

        /* accumulate contributions to the final sequence */
        for (idx_t m = 0; m <= m_max; ++m) {
            idx_t n = t + m;
            seq_t add = _poly[m] * (seq_t)n;
            add /= (seq_t)t;           /* guaranteed integer division */
            _A[n] += add;
        }
    }

    _computed_max = N;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _compute_to(n);
    return _A[n];
}
