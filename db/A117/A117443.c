/*
 * A117443  Expansion of e.g.f.: exp(x)/(cos(x) + sin(x)).
 * https://oeis.org/A117443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117443: a(n) = exp(x)/(cos(x)+sin(x)) coefficient expansion
 *
 * Recurrence (derived from (cos x + sin x) * G(x) = e^x with
 * G(x) = Σ a(n) x^n / n!):
 *
 *   a(0) = 1
 *   for n ≥ 1:
 *     a(n) = 1 − Σ_{k=1..n} binomial(n, k) * a(n−k) * (−1)^{⌊k/2⌋}
 *
 * The recurrence is applied iteratively, storing already‑computed values
 * so that repeated calls of a(idx_t n) with arbitrary n stay O(n^2) total.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* idx_t and seq_t are defined by the build system. */
typedef uint64_t idx_t;   /* index type */

/* static storage for memoisation */
static seq_t *a_seq   = NULL;   /* array of coefficients a(n) */
static size_t a_cap = 0;        /* allocated capacity (number of elements) */
static idx_t  a_last = (idx_t)-1;  /* largest n that has been computed */

/*----------------------------------------------------------------------------*/
/* ensure that a_seq has room for indices 0..n inclusive                       */
static void
ensure_a_size(idx_t n)
{
    if ((size_t)(n + 1u) <= a_cap)        /* already adequate */
        return;

    size_t new_cap = a_cap ? a_cap : 1u;  /* start with 1 element */
    while (new_cap < (size_t)(n + 1u))
        new_cap <<= 1;                    /* double until big enough */

    seq_t *new_ptr = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* minimal error handling – terminate on allocation failure */
        abort();
    }
    a_seq = new_ptr;
    a_cap = new_cap;
}

/*----------------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    /* make sure storage is large enough */
    ensure_a_size(n);

    /* first call: initialise a(0) = 1 */
    if (a_last == (idx_t)-1) {
        a_seq[0] = (seq_t)1;
        a_last   = 0;
    }

    /* compute any missing values up to n */
    for (idx_t m = a_last + 1u; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        /* binomial coefficient bin = C(m, 0) = 1 */
        seq_t bin = (seq_t)1;

        for (idx_t k = 1u; k <= m; ++k) {
            /* C(m,k) = C(m,k-1) * (m-k+1) / k  (exact integer division) */
            bin = bin * ((seq_t)(m - k + 1u)) / (seq_t)k;

            /* sign = (−1)^{⌊k/2⌋}  -> pattern +,+,−,−,+,+,... */
            seq_t sign = (((k / 2u) & 1u) ? (seq_t)-1 : (seq_t)1);

            /* term = sign * bin * a(m−k) */
            size_t idx = (size_t)(m - k);
            seq_t term = sign * bin * a_seq[idx];

            sum += term;
        }

        /* a(m) = 1 − sum */
        a_seq[m] = ((seq_t)1) - sum;
    }

    a_last = n;
    return a_seq[n];
}
