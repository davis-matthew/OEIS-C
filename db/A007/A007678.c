/*
 * A007678  Number of regions in regular n-gon with all diagonals drawn.
 * https://oeis.org/A007678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Poonen-Rubinstein (see A006533 Mma), with d_k(n) = [k | n]: a(n)
 *            = (n^4-6n^3+23n^2-42n+24)/24 + (-5n^3+42n^2-40n-48)/48*d_2 -
 *            (3n/4)*d_4 + (-53n^2+310n)/12*d_6 + (49n/2)*d_12 + 32n*d_18 +
 *            19n*d_24 - 36n*d_30 - 50n*d_42 - 190n*d_60 - 78n*d_84 - 48n*d_90
 *            - 78n*d_120 - 48n*d_210, for n >= 3; a(1) =...
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/from_i128.h"

static const int dv[10] = {12, 18, 24, 30, 42, 60, 84, 90, 120, 210};
static const int cf[10] = {49*24, 32*48, 19*48, -36*48, -50*48, -190*48, -78*48, -48*48, -78*48, -48*48};

seq_t a(idx_t n) {
    int i;
    if (n <= 2) return 0;
    if (n < ((idx_t)1 << 24)) {
        __int128 m = (__int128)n;
        __int128 t = 2 * (m*m*m*m - 6*m*m*m + 23*m*m - 42*m + 24);
        if (n % 2 == 0) t += -5*m*m*m + 42*m*m - 40*m - 48;
        if (n % 4 == 0) t -= 36*m;
        if (n % 6 == 0) t += 4 * (-53*m*m + 310*m);
        for (i = 0; i < 10; i++)
            if (n % (idx_t)dv[i] == 0) t += (__int128)cf[i] * m;
        return from_i128(t / 48);
    } else {
        seq_t m = (seq_t)n;
        seq_t m2 = m * m, m3 = m2 * m, m4 = m3 * m;
        seq_t P = 2 * (m4 + 23*m2 + 24);
        seq_t N = 2 * (6*m3 + 42*m);
        if (n % 2 == 0) { P += 42*m2; N += 5*m3 + 40*m + 48; }
        if (n % 4 == 0) N += 36*m;
        if (n % 6 == 0) { P += 1240*m; N += 212*m2; }
        for (i = 0; i < 10; i++) {
            if (n % (idx_t)dv[i] == 0) {
                if (cf[i] > 0) P += (seq_t)cf[i] * m;
                else N += (seq_t)(-cf[i]) * m;
            }
        }
        return (P - N) / 48;
    }
}
