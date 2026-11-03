/*
 * A162208  Number of reduced words of length n in the Weyl group D_5.
 * https://oeis.org/A162208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of the polynomial f(5)*f(2)*f(4)*f(6)*f(8),
 *            where f(m) = (1 - x^m)/(1 - x). The Weyl group D5 has a longest
 *            word of length 20, so a(n) = 0 for n > 20
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: full, pass, 21 terms through n = 20 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Multiply polynomial 'poly' by factor f(m) = 1 + x + ... + x^(m-1).
   'deg' is the current degree of poly, updated to new degree. */
static void multiply_factor(seq_t *poly, size_t *deg, size_t m)
{
    seq_t tmp[21] = {0};
    size_t new_deg = *deg + m - 1;   /* resulting degree after multiplication */
    for (size_t i = 0; i <= *deg; ++i) {
        for (size_t j = 0; j < m; ++j) {
            tmp[i + j] += poly[i];
        }
    }
    for (size_t i = 0; i <= new_deg; ++i) {
        poly[i] = tmp[i];
    }
    *deg = new_deg;
}

static bool initialized = false;
static seq_t coeff[21];  /* coefficients for degrees 0..20 */

static void init_coeff(void)
{
    size_t deg = 0;
    /* Start with polynomial 1. */
    coeff[0] = 1;
    /* Apply the factors in the order described: f(5), f(2), f(4), f(6), f(8). */
    multiply_factor(coeff, &deg, 5);
    multiply_factor(coeff, &deg, 2);
    multiply_factor(coeff, &deg, 4);
    multiply_factor(coeff, &deg, 6);
    multiply_factor(coeff, &deg, 8);
    /* Remaining coeff[i] for i > 20 remain zero (static initialization). */
    initialized = true;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_coeff();
    }
    if (n <= 20) {
        return coeff[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
