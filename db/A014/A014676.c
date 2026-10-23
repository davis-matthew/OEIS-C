/*
 * A014676  Inverse of 667th cyclotomic polynomial.
 * https://oeis.org/A014676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of xⁿ in the power‑series expansion of
 *            1/Φ₆₆₇(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Φ₆₆₇(x) = (x¹⁰⁰‑1)(x‑1) / ((x²³‑1)(x²⁹‑1)), degree 616; */
/* The sequence is periodic with period 667. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Pre‑computed values of a(n) for 0 ≤ n < 667 */
static seq_t a_cache[667];
static bool a_initialized = false;

/* Compute the coefficients of Φ₆₆₇(x) and the first period of its inverse. */
static void init_a(void)
{
    const size_t PERIOD = 667;
    const size_t DEG   = 616;          /* degree of Φ₆₆₇ */

    /* (x²³‑1)(x²⁹‑1) = x⁵² – x²⁹ – x²³ + 1 */
    seq_t Q[53] = {0};
    Q[0]  =  1;
    Q[23] = -1;
    Q[29] = -1;
    Q[52] =  1;      /* leading coefficient is 1 */

    /* (x⁶⁶⁷‑1)(x‑1) = x⁶⁶⁸ – x⁶⁶⁷ – x + 1 */
    seq_t P[669];
    for (size_t i = 0; i < 669; ++i) P[i] = 0;
    P[0]  =  1;
    P[1]  = -1;
    P[667] = -1;
    P[668] =  1;

    /* Divide P by Q to obtain Φ₆₆₇(x) */
    seq_t phi[617] = {0};   /* coefficients of Φ₆₆₇, indices 0..616 */

    for (int k = 668; k >= 52; --k) {
        seq_t coeff = P[k];
        if (coeff == 0) continue;
        size_t idx = (size_t)(k - 52);      /* coefficient position in φ */

        phi[idx] = coeff;

        /* Subtract coeff * Q from the current P remainder */
        P[idx]       -= coeff * Q[0];   /* Q[0] = 1 */
        P[idx + 23] -= coeff * Q[23];   /* Q[23] = –1 */
        P[idx + 29] -= coeff * Q[29];   /* Q[29] = –1 */
        P[idx + 52] -= coeff * Q[52];   /* Q[52] =  1 */
    }

    /* Compute the inverse power‑series coefficients a[0..666] */
    a_cache[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t max_k = (n < DEG) ? n : DEG;
        for (size_t k = 1; k <= max_k; ++k) {
            sum += phi[k] * a_cache[n - k];
        }
        a_cache[n] = -sum;
    }

    a_initialized = true;
}

seq_t a(idx_t n)
{
    if (!a_initialized)
        init_a();
    return a_cache[(size_t)(n % 667)];
}
