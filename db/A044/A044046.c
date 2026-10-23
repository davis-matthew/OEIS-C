/*
 * A044046  Nonisomorphic catacondensed monoheptafusenes (see reference for
 *          precise definition).
 * https://oeis.org/A044046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044046: Nonisomorphic catacondensed monoheptafusenes
   Formula: a(n) = ( [z^n](U(z)^2 + U(z^2)) ) / 2,
   with U(z) = (1 - 3z - sqrt(1 - 6z + 5z^2)) / (2z).
   The implementation expands the series for sqrt(1-6z+5z^2),
   derives U(z) coefficients, then evaluates the required
   convolution to obtain a(n).  All computations are performed
   on the type seq_t; the code works for any integer size
   provided by the build (8,16,32,64,128‑bit or arbitrary‑precision). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate working space for the series up to n */
    size_t size_s = (size_t)n + 2;          /* indices 0 .. n+1  */
    size_t size_u = (size_t)n + 1;          /* indices 0 .. n    */
    size_t size_c = (size_t)n + 1;          /* conv[0..n]        */

    seq_t *s_abs = (seq_t*)calloc(size_s, sizeof(seq_t));
    seq_t *u     = (seq_t*)calloc(size_u, sizeof(seq_t));
    seq_t *conv  = (seq_t*)calloc(size_c, sizeof(seq_t));

    if (!s_abs || !u || !conv) {
        /* Memory allocation failed – return zero silently. */
        free(s_abs); free(u); free(conv);
        return 0;
    }

    /* Initial values for |s_n| :   s0 = 1, s1 = 3, s2 = 2   (all positive) */
    s_abs[0] = 1;
    if (n >= 1) s_abs[1] = 3;
    if (n >= 2) s_abs[2] = 2;

    /* Compute |s_n| for n = 3 .. n+1 */
    for (size_t m = 3; m <= (size_t)n + 1; ++m) {
        seq_t sum = 0;
        for (size_t k = 1; k <= m - 1; ++k) {
            sum += s_abs[k] * s_abs[m - k];
        }
        s_abs[m] = sum / 2;             /* sum is even */
    }

    /* Convert to u[n]  :  u[0] = 0 ,  u[n] = |s_{n+1}| / 2 for n>=1 */
    u[0] = 0;
    for (size_t i = 1; i <= (size_t)n; ++i) {
        u[i] = s_abs[i + 1] / 2;
    }

    /* Conviment: conv[k] = sum_{i=0}^{k} u_i * u_{k-i}   (k <= n) */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        for (size_t j = 0; j + i <= (size_t)n; ++j) {
            conv[i + j] += u[i] * u[j];
        }
    }

    /* Final formula:  a(n) = ( conv[n] + (n even ? u[n/2] : 0) ) / 2 */
    seq_t result = conv[(size_t)n];
    if ((n % 2) == 0) {
        result += u[(size_t)(n / 2)];
    }
    result = result / 2;

    free(s_abs); free(u); free(conv);
    return result;
}
