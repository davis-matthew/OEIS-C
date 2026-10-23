/*
 * A162179  Number of reduced words of length n in the Weyl group B_42.
 * https://oeis.org/A162179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in Prod_{k=1..42} (1 - x^(2k))/(1
 *            - x) (i.e. the product over k of (1 + x + ... + x^(2k-1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 1764   /* 42*42, maximal total degree */
#define M    42      /* the highest index in the product */

static seq_t coeff[MAX_N + 1];
static seq_t tmp[MAX_N + 1];
static bool initialized = false;

/* Builds all coefficients of the generating polynomial once. */
static void init_coeffs(void)
{
    if (initialized)
        return;

    /* Set all coefficients to zero and start with the constant term 1. */
    for (size_t i = 0; i <= MAX_N; ++i)
        coeff[i] = (seq_t)0;
    coeff[0] = (seq_t)1;

    size_t max_deg = 0;           /* highest degree presently represented */

    for (size_t k = 1; k <= M; ++k) {
        size_t len = 2 * k;       /* factor has terms 0..2k-1, so length 2k */

        size_t new_deg = max_deg + len - 1;
        /* Zero temporary buffer for the new coefficients. */
        for (size_t i = 0; i <= new_deg; ++i)
            tmp[i] = (seq_t)0;

        /* Convolve the current polynomial with the new factor. */
        for (size_t i = 0; i <= max_deg; ++i) {
            seq_t v = coeff[i];
            if (v == (seq_t)0)
                continue;
            for (size_t t = 0; t < len; ++t) {
                tmp[i + t] += v;
            }
        }

        /* Move the result back to coeff. */
        for (size_t i = 0; i <= new_deg; ++i)
            coeff[i] = tmp[i];

        max_deg = new_deg;
    }

    initialized = true;
}

/* Public function: returns the nth term of A162179. */
seq_t a(idx_t n)
{
    init_coeffs();

    if ((size_t)n > MAX_N)
        return (seq_t)0;
    return coeff[(size_t)n];
}
