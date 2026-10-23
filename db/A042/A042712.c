/*
 * A042712  Numerators of continued fraction convergents to sqrt(886).
 * https://oeis.org/A042712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the continued fraction convergents to sqrt(886) */
#include <stdint.h>

/* Periodic part of the continued fraction for sqrt(886), length 54
   a0 = 29, followed by a1..a54 with a54 = 2*a0 = 58 */
static const uint32_t period[54] = {
    1, 3, 3, 1, 2, 1, 1, 5, 2, 1, 1, 1, 9, 3, 2, 1, 1, 19, 3, 1,
    11, 6, 1, 1, 7, 1, 28, 1, 7, 1, 1, 6, 11, 1, 3, 19, 1, 1,
    2, 3, 9, 1, 1, 1, 2, 5, 1, 1, 2, 1, 3, 3, 1, 58
};

seq_t a(idx_t n)
{
    /* Handling n=0 directly by the recurrence */
    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k;
        if (k == 0)
            a_k = (seq_t)29;          /* a0 */
        else
            a_k = (seq_t)period[(k - 1) % 54];  /* periodic part */

        seq_t p_k = a_k * p_minus1 + p_minus2; /* p_k = a_k * p_{k-1} + p_{k-2} */

        p_minus2 = p_minus1;
        p_minus1 = p_k;
    }
    return p_minus1;  /* p_n */
}
