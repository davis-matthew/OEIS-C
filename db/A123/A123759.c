/*
 * A123759  Expansion of f(-q)*psi(-q^5) in powers of q where f(), psi() are
 *          Ramanujan theta functions.
 * https://oeis.org/A123759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A123759: coefficient a(n) of the product 
 * ∏_{k>0}(1 - x^k)(1 - x^(5k))(1 + x^(10k)) expanded as a power series in x.
 * a(n) = coefficient of x^n in the series.
 */
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 separately to avoid allocating a zero-sized array. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Convert the 64‑bit index to size_t for array indexing. */
    size_t N = (size_t)n;

    /* Allocate an array of seq_t values initialized to 0. */
    seq_t *coeff = seq_alloc_array(N + 1);
    if (!coeff) {
        /* Allocation failed – return zero as a graceful fallback. */
        return (seq_t)0;
    }

    /* Initialise coefficients. */
    coeff[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) {
        coeff[i] = (seq_t)0;
    }

    /* Multiply by (1 - x^k) for k = 1 .. N. */
    for (size_t k = 1; k <= N; ++k) {
        size_t off = k;
        for (size_t j = N; j >= off; --j) {
            coeff[j] -= coeff[j - off];
            if (j == off) break;      /* avoid unsigned underflow */
        }
    }

    /* Multiply by (1 - x^(5k)) for k = 1 .. ⌊N/5⌋. */
    for (size_t k = 1; k <= N / 5; ++k) {
        size_t off5 = 5 * k;
        for (size_t j = N; j >= off5; --j) {
            coeff[j] -= coeff[j - off5];
            if (j == off5) break;
        }
    }

    /* Multiply by (1 + x^(10k)) for k = 1 .. ⌊N/10⌋. */
    for (size_t k = 1; k <= N / 10; ++k) {
        size_t off10 = 10 * k;
        for (size_t j = N; j >= off10; --j) {
            coeff[j] += coeff[j - off10];
            if (j == off10) break;
        }
    }

    seq_t result = coeff[N];
    seq_free_array(coeff);
    return result;
}
