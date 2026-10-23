/*
 * A120870  a(n) is the number k for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 13*n^2.
 * https://oeis.org/A120870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (((n*sqrt(13))/2 + 0.5) rounded down + 0½)^2 - (13/4)n²
 *            for odd n; * a(n) = (1 + floor((n*sqrt(13))/2))² - (13/4)n² for
 *            even n * (reformulated using integer arithmetic): * a(n) =
 *            ((2*t+1)² - 13 n²)/4 , t = floor((n*sqrt(13)+1)/2) for odd n *
 *            a(n) = (4*(1+u)² - 13 n²)/4 , u = floor(n*sqrt(1...
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n must be ≥ 1; behaviour for n == 0 is undefined */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                  /* n² */
    seq_t s  = floor_sqrt((seq_t)13 * n2); /* floor(n*sqrt(13)) */

    if (n & 1) {                         /* odd n */
        seq_t t   = (s + 1) / 2;         /* floor((n*sqrt(13)+1)/2) */
        seq_t val = (seq_t)2 * t + 1;    /* 2*t + 1 */
        seq_t num = val * val - (seq_t)13 * n2;
        return num / 4;
    } else {                             /* even n */
        seq_t u   = s / 2;               /* floor(n*sqrt(13)/2) */
        seq_t v   = u + 1;               /* 1 + u */
        seq_t num = 4 * v * v - (seq_t)13 * n2;
        return num / 4;
    }
}
