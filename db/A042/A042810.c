/*
 * A042810  Numerators of continued fraction convergents to sqrt(936).
 * https://oeis.org/A042810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042810: Numerators of continued fraction convergents to sqrt(936) */
/* Recurrence: a(n) = 10402*a(n-8) - a(n-16), for n >= 16 */
/* Initial terms: a(0) = 30, a(1) = 31, a(2) = 61, a(3) = 153, a(4) = 979,
 * a(5) = 2111, a(6) = 3090, a(7) = 5201, a(8) = 315150, a(9) = 320351,
 * a(10) = 635501, a(11) = 1591353, a(12) = 10183619, a(13) = 21958591,
 * a(14) = 32142210, a(15) = 54100801 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
static seq_t init[16];
static bool init_done = false;

/* Compute the first 16 convergent numerators of sqrt(936). */
static void compute_initial_terms(void)
{
    const idx_t D = 936;
    /* floor(sqrt(D)) */
    idx_t sqrtD = 0;
    while ((sqrtD + 1) * (sqrtD + 1) <= D)
        ++sqrtD;

    idx_t m = 0, d = 1;
    idx_t a0 = sqrtD;
    idx_t a = a0;
    idx_t partials[16];

    for (size_t k = 0; k < 16; ++k) {
        partials[k] = a;

        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* Build convergent numerators: p[-2]=0, p[-1]=1, p[k] = a_k*p[k-1]+p[k-2] */
    seq_t p_nm2 = 0; /* p[-2] */
    seq_t p_nm1 = 1; /* p[-1] */

    for (size_t k = 0; k < 16; ++k) {
        seq_t pk = (seq_t)partials[k] * p_nm1 + p_nm2;
        init[k] = pk;
        p_nm2 = p_nm1;
        p_nm1 = pk;
    }
    init_done = true;
}

/* Return the n-th term of the sequence (0‑based). */
seq_t a(idx_t n)
{
    if (!init_done)
        compute_initial_terms();

    if (n < 16)          /* return an initial term directly */
        return init[(size_t)n];

    /* Circular buffer holding the last 16 values. */
    seq_t buf[16];
    for (size_t i = 0; i < 16; ++i)
        buf[i] = init[i];

    /* Compute terms from 16 up to n. */
    for (idx_t i = 16; i <= n; ++i) {
        /* Use mask 15 for modulus 16 (power of two). */
        seq_t val = (seq_t)10402 * buf[(i - 8) & 15] - buf[(i - 16) & 15];
        buf[i & 15] = val;
    }

    return buf[n & 15];
}
