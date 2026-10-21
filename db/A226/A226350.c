/*
 * A226350  Expansion of psi(x) * psi(-x^3) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A226350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A226350 – Expansion of ψ(x)·ψ(–x³) in powers of x
    Recurrence (Euler transform):
        a(0) = 1
        for n ≥ 1:
            a(n) = (1/n) · Σ_{k=1..n} h(k) · a(n–k)
    where
        h(k) = Σ_{d|k} d · b(d)
        b(d) is the period‑12 sequence
              [1, –1, 0, –1, 1, –1, 1, –1, 0, –1, 1, –2]
    The implementation memoises all previously computed values so that
    each call to a(n) is O(n) at worst and never recomputes smaller
    terms.  It works for the integer types supplied by the build
    (seq_t, idx_t).  No floating‑point or external libraries are used.   */

#include <stdlib.h>
#include <stddef.h>

/* Periodic sequence b(d).  Index 0 is unused. */
static const int B[13] = {
    0,  /* unused 0 */
    1, -1, 0, -1, 1, -1, 1, -1, 0, -1, 1, -2
};

/* Memoised arrays for a(n) and h(n). */
static seq_t *A = NULL;   /* a[0] … a[last]          */
static seq_t *H = NULL;   /* h[0] … h[last]          */
static size_t cap = 0;    /* current allocation size */
static size_t last = 0;   /* largest index that has been computed */

/* Ensure the internal arrays can hold indices 0 … n.  The buffer is
   grown geometrically to keep the amortised cost O(1). */
static void ensure_capacity(size_t n)
{
    if (cap <= n) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *newA = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        seq_t *newH = (seq_t*)realloc(H, new_cap * sizeof(seq_t));
        if (!newA || !newH) abort();          /* allocation failure */
        A = newA; H = newH;
        cap = new_cap;
    }
}

/* Compute h(k) for k = start … limit inclusive.  H[0] is unused
   (manually kept at 0). */
static void compute_h_range(size_t start, size_t limit)
{
    for (size_t k = start; k <= limit; ++k) {
        seq_t sum = 0;
        for (size_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int bd = B[(d - 1) % 12 + 1];
                sum += (seq_t)d * (seq_t)bd;
                size_t d2 = k / d;
                if (d2 != d) {
                    int bd2 = B[(d2 - 1) % 12 + 1];
                    sum += (seq_t)d2 * (seq_t)bd2;
                }
            }
        }
        H[k] = sum;
    }
}

/* Compute a(k) for k = start … limit inclusive.  The recurrence
   requires all h(1..k) and a(0..k‑1) to be known. */
static void compute_a_range(size_t start, size_t limit)
{
    for (size_t i = start; i <= limit; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= i; ++k) {
            sum += H[k] * A[i - k];
        }
        A[i] = sum / (seq_t)i;   /* division is exact */
    }
}

/* Public function required by the specifications. */
seq_t a(idx_t n)
{
    /* n is unsigned; cast to size_t for array indexing. */
    size_t idx = (size_t)n;

    /* First‑time initialisation of a(0) = 1, h(0) = 0. */
    if (cap == 0) {
        ensure_capacity(0);
        A[0] = (seq_t)1;
        H[0] = (seq_t)0;
        last  = 0;
    }

    /* Extend the memoised range if necessary. */
    if (last < idx) {
        ensure_capacity(idx);
        /* Compute missing h values, then a values. */
        compute_h_range(last + 1, idx);
        compute_a_range(last + 1, idx);
        last = idx;
    }

    /* Return the cached value. */
    return A[idx];
}
