/*
 * A042554  Numerators of continued fraction convergents to sqrt(806).
 * https://oeis.org/A042554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(806)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/*
 * Continued fraction expansion of sqrt(806):
 * sqrt(806) = [28; 2,1,1,3,2,5,4,5,2,3,1,1,2,56] with period 14.
 *
 * The numerators of the convergents (h_n) satisfy
 *     h_-2 = 0,   h_-1 = 1
 *     h_n   = a_n * h_(n-1) + h_(n-2)
 * where a_0 = 28 and a_1..a_14 are the periodic partial quotients above.
 * a(n) returns h_n.
 */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction starting from a_1 */
    static const seq_t period[14] = {
        (seq_t)2,  (seq_t)1,  (seq_t)1,  (seq_t)3,
        (seq_t)2,  (seq_t)5,  (seq_t)4,  (seq_t)5,
        (seq_t)2,  (seq_t)3,  (seq_t)1,  (seq_t)1,
        (seq_t)2,  (seq_t)56
    };
    const seq_t a0 = (seq_t)28;          /* a_0 = floor(sqrt(806)) */

    seq_t h_nm2 = (seq_t)0; /* h_{-2} */
    seq_t h_nm1 = (seq_t)1; /* h_{-1} */
    seq_t h_n   = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? a0 : period[(i - 1) % 14];
        h_n = ai * h_nm1 + h_nm2;
        h_nm2 = h_nm1;
        h_nm1 = h_n;
    }
    return h_n;
}
