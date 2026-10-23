/*
 * A158196  Expansion of (1-x^2*c(x)^4)/(1-3*x*c(x)^2), c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A158196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the coefficient of xⁿ in (1 – x²·C(x)⁴)/(1 – 3x·C(x)²),
 *            where C(x) is the Catalan generating function. The recurrence
 *            used below is derived from this generating function: for n≥0
 *            a(0) = 1 a(n) = Σ_{k=1}^{n} a(n‑k)·(3·b_{k‑1}) – (n≥2 ? e_{n‑2}
 *            : 0), with b_k = Σ_{i=0}^{k} c_i·c_{k‑i} (c_k...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdlib.h>
#include <stdint.h>

/* Dependencies for arbitrary‑precision types are provided by the build system.
   All required type definitions and helper functions are already available. */

/* Static storage for sequence values and the auxiliary Catalan sequences. */
static seq_t *cat = NULL; /* Catalan numbers c_k */
static seq_t *b   = NULL; /* b_k = c * c */
static seq_t *e   = NULL; /* e_k = b * b */
static seq_t *seq = NULL; /* a_k */

/* Current number of elements computed (-1 means uninitialised). */
static idx_t max_n = (idx_t)-1;

/* Storage capacities for the dynamic arrays. */
static size_t cap_cat = 0;
static size_t cap_b   = 0;
static size_t cap_e   = 0;
static size_t cap_seq = 0;

/* Helper: resize an array to accommodate at least `need` elements. */
static void resize_array(seq_t **p, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory allocation failed */
    *p = tmp;
    *cap = new_cap;
}

/* Expand the pre‑computed tables so that they contain values up to index `n`. */
static void ensure_computed(idx_t n)
{
    if ((intmax_t)n <= (intmax_t)max_n)
        return;                       /* already available */

    /* Allocate or grow arrays. */
    if (cat == NULL) {
        resize_array(&cat, &cap_cat, n + 1);
        resize_array(&b,   &cap_b,   n + 1);
        resize_array(&e,   &cap_e,   n + 1);
        resize_array(&seq, &cap_seq, n + 1);

        /* Base cases. */
        cat[0] = (seq_t)1;          /* C_0 = 1 */
        b[0]   = (seq_t)1;          /* b_0 = 1 */
        e[0]   = (seq_t)1;          /* e_0 = 1 */
        seq[0] = (seq_t)1;          /* a_0 = 1 */
        max_n   = 0;
    }
    else {
        /* Extend arrays to required size if necessary. */
        resize_array(&cat, &cap_cat, n + 1);
        resize_array(&b,   &cap_b,   n + 1);
        resize_array(&e,   &cap_e,   n + 1);
        resize_array(&seq, &cap_seq, n + 1);
    }

    /* Compute new Catalan numbers. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 0; i < k; ++i)
            sum += cat[i] * cat[k - 1 - i];
        cat[k] = sum;
    }

    /* Compute new b_k = c * c. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= k; ++i)
            sum += cat[i] * cat[k - i];
        b[k] = sum;
    }

    /* Compute new e_k = b * b. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= k; ++i)
            sum += b[i] * b[k - i];
        e[k] = sum;
    }

    /* Compute new a_k using the recurrence. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; ++j) {
            seq_t coeff = (seq_t)3 * b[j - 1];
            sum += seq[k - j] * coeff;
        }
        if (k >= 2)
            sum -= e[k - 2];
        seq[k] = sum;
    }

    max_n = n;
}

/* External function required by the platform. */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return seq[n];
}
