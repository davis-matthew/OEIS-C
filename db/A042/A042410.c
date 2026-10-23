/*
 * A042410  Numerators of continued fraction convergents to sqrt(733).
 * https://oeis.org/A042410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(733).  
   Computation uses the recurrence  
        p_{-2} = 0,  p_{-1} = 1
        p_k   = a_k * p_{k-1} + p_{k-2}   (k ≥ 0)  
   where a_0 = 27 and the periodic part a_1 … a_5 = 13, 1, 1, 13, 54 repeats. */
#include <stddef.h>

seq_t a(idx_t n) {
    /* Periodic coefficients after the initial term: 13,1,1,13,54 */
    static const int period[] = {13, 1, 1, 13, 54};
    const size_t period_len = 5;

    seq_t p_minus2 = 0; /* p_{-2} */
    seq_t p_minus1 = 1; /* p_{-1} */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)27;
        } else {
            size_t idx = (size_t)((k - 1) % period_len);
            ak = (seq_t)period[idx];
        }
        seq_t p_k = ak * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_k;
    }
    return p_minus1;
}
