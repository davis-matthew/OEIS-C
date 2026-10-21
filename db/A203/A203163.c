/*
 * A203163  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,3,4,1,2,3,4,1,2,3,4,...) = A010883.
 * https://oeis.org/A203163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^⌊n/4⌋ * P[n%4] * (⌊n/4⌋*25 + D[n%4]) / 12 * where
 *            P[0]=1, P[1]=1, P[2]=2, P[3]=6 * and D[0]=0, D[1]=12, D[2]=18,
 *            D[3]=22
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n / 4;      /* floor division */
    int r = n % 4;        /* remainder 0..3 */

    seq_t pref;
    unsigned d;
    switch (r) {
        case 0: pref = 1; d = 0; break;
        case 1: pref = 1; d = 12; break;
        case 2: pref = 2; d = 18; break;
        case 3: pref = 6; d = 22; break;
    }

    /* Compute 24^m */
    seq_t base = (seq_t)1;
    for (idx_t i = 0; i < m; ++i)
        base *= (seq_t)24;

    /* Factor (m*25 + d) */
    seq_t factor = ((seq_t)m * (seq_t)25) + (seq_t)d;

    if (m == 0) {
        /* For m = 0 we cannot divide base by 12 */
        seq_t num = base * pref;
        num *= factor;
        return num / (seq_t)12;
    } else {
        /* For m >= 1, base is divisible by 12 */
        seq_t baseDiv = base / (seq_t)12;
        seq_t res = baseDiv * pref;
        res *= factor;
        return res;
    }
}
