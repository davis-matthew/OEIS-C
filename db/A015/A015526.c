/*
 * A015526  Inverse of 1517th cyclotomic polynomial.
 * https://oeis.org/A015526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in the formal power series inverse of
 *            the 1517th cyclotomic polynomial Φ₁₅₁₇(x). The coefficients are
 *            obtained from the recurrence Φ₁₅₁₇(x) · A(x) = 1 where A(x) =
 *            Σ_{n≥0} a(n) xⁿ. The polynomial Φ₁₅₁₇ is computed once via the
 *            identity Φ₁₅₁₇(x) = (x¹⁵¹⁸ – x¹⁵¹⁷ – x + 1)...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define PHI_DEG 1440            /* φ(1517) = 36·40 */

static seq_t *coeff   = NULL;   /* coefficients of Φ₁₅₁₇, indexed 0 … PHI_DEG */
static bool   coeff_ok = false; /* have we computed coeff */

static void
init_coeffs(void)
{
    if (coeff_ok) return;

    coeff = (seq_t*)malloc((PHI_DEG + 1) * sizeof(seq_t));
    if (!coeff) abort();          /* allocation failure */

    for (size_t k = 0; k <= PHI_DEG; ++k) {
        seq_t N = 0;
        if (k == 0) N = 1;
        else if (k == 1) N = -1;
        /* N_k is 0 for all other k */

        seq_t val = N;
        if (k >= 37) val += coeff[k - 37];
        if (k >= 41) val += coeff[k - 41];
        if (k >= 78) val -= coeff[k - 78];   /* D_78 = +1 */

        coeff[k] = val;
    }
    coeff_ok = true;
}

/* cache of a(n) values */
static seq_t *cache   = NULL;   /* array of computed a(n) */
static size_t cache_len = 0;    /* elements stored (end index + 1) */
static size_t cache_cap = 0;    /* allocated capacity */

static void
ensure_cache(size_t n)
{
    /* enlarge cache if necessary */
    if (n < cache_len) return;

    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();         /* allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;

    /* fill missing terms */
    init_coeffs();                 /* make sure coeff is ready */

    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        size_t kmax = i < PHI_DEG ? i : PHI_DEG;
        for (size_t k = 1; k <= kmax; ++k)
            sum += coeff[k] * cache[i - k];
        cache[i] = -sum;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
