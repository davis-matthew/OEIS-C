/*
 * A220707  Number of ways to reciprocally link elements of an n X 7 array
 *          either to themselves or to exactly two horizontal and antidiagonal
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+6*a(n-2)-17*a(n-3)-19*a(n-4)+10*a(n-5)+16*a(n-6)
 *            -46*a(n-7)+7*a(n-8)+15*a(n-9)-14*a(n-10)+21*a(n-11)+3*a(n-12)+10*a(n-13)
 *            +8*a(n-14)+2*a(n-15)+5*a(n-16)+a(n-17)+2*a(n-18)-a(n-19)-a(n-20)
 *            (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 20 initial terms (a(1) .. a(20)) provided by the OEIS entry */
    static const seq_t init[20] = {
        1ULL,
        13ULL,
        48ULL,
        298ULL,
        1520ULL,
        8338ULL,
        44976ULL,
        244029ULL,
        1322551ULL,
        7171769ULL,
        38885648ULL,
        210854845ULL,
        1143330649ULL,
        6199601829ULL,
        33616711696ULL,
        182283373322ULL,
        988413912488ULL,
        5359579124490ULL,
        29061800830768ULL,
        157584813722329ULL
    };

    if (n <= 20) return init[n - 1];

    /* Recurrence coefficients for k = 1..20  (coeff[0] = coefficient of a(n-1)) */
    static const int coeff[20] = {
        5, 6, -17, -19, 10, 16, -46, 7, 15, -14,
        21, 3, 10, 8, 2, 5, 1, 2, -1, -1
    };

    /* Sliding window of the last 20 values, index 0 holds a(current-19),
       index 19 holds a(current-0) (most recent)   */
    seq_t buff[20];
    for (int i = 0; i < 20; ++i) buff[i] = init[i];

    for (idx_t i = 21; i <= n; ++i) {
        seq_t term = 0;
        for (int k = 1; k <= 20; ++k) {
            int c = coeff[k - 1];
            seq_t v = buff[20 - k];
            if (c >= 0) {
                term += (seq_t)c * v;
            } else {
                term -= (seq_t)(-c) * v;
            }
        }
        /* shift window to discard the oldest value and append the new one */
        for (int j = 0; j < 19; ++j) buff[j] = buff[j + 1];
        buff[19] = term;
    }
    return buff[19];
}
