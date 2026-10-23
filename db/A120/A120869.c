/*
 * A120869  a(n) is the number j for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 13*n^2.
 * https://oeis.org/A120869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n even (n = 2m): a(n) = 13*m^2 - floor(m*√13)^2 For n odd (n
 *            = 2m+1): a(n) = 13*m^2 + 13*m + 3 - floor((floor(n*√13)+1)/2)^2
 *            + floor((floor(n*√13)+1)/2) The function uses integer arithmetic
 *            only, relying on the provided isqrt_seq routine for
 *            floor(sqrt())
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0, return 0 */

    if ((n & 1) == 0) {        /* even n = 2m */
        idx_t m = n >> 1;
        seq_t mm = (seq_t)m * (seq_t)m;          /* m^2 */
        seq_t val = 13 * mm;                     /* 13*m^2 */
        seq_t s = isqrt_seq(mm * 13);            /* floor(m*√13) */
        return val - (s * s);                   /* 13*m^2 - s^2 */
    } else {                    /* odd n = 2m+1 */
        idx_t m = n >> 1;                         /* floor(n/2) */
        seq_t mm = (seq_t)m * (seq_t)m;          /* m^2 */
        seq_t term1 = 13 * mm + 13 * (seq_t)m + 3;/* 13*m^2 + 13*m + 3 */

        seq_t q = isqrt_seq(((seq_t)n * (seq_t)n) * 13); /* floor(n*√13) */
        seq_t t = (q + 1) / 2;                     /* floor((q+1)/2) */

        return term1 - (t * t) + t;               /* final expression */
    }
}
