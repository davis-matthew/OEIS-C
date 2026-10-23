/*
 * A027918  Least k such that 1+2+...+k >= E{1,2,...,n}, where E is the 4th
 *          elementary symmetric function.
 * https://oeis.org/A027918
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : k(k+1)/2 >= A000915(n)}
 * Range:     u8 none, u16 n <= 6, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A000915(n) is the 4th elementary symmetric sum of {1,2,...,n}. */
/* Computation uses Newton identities up to the 4th power sum. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* values below the offset are undefined */

    /* Use n itself as the size of the set {1..n} */
    seq_t m = (seq_t)n;

    /* Power sums S1..S4 for the set {1..m} */
    seq_t S1 = m * (m + 1) / 2;                               /* sum i               */
    seq_t S2 = m * (m + 1) * (2 * m + 1) / 6;                 /* sum i^2             */
    seq_t S3 = S1 * S1;                                       /* (sum i)^2 = sum i^3 for consecutive integers */
    seq_t t  = 3 * m * m + 3 * m - 1;                         /* 3m^2 + 3m - 1       */
    seq_t S4 = m * (m + 1) * (2 * m + 1) * t / 30;           /* sum i^4             */

    /* Elementary symmetric sums via Newton's identities */
    seq_t e2 = (S1 * S1 - S2) / 2;                            /* e2 */
    seq_t e3 = (S1 * e2 - S2 * S1 + S3) / 3;                  /* e3 */
    seq_t e4 = (S1 * e3 - S2 * e2 + S3 * S1 - S4) / 4;        /* e4 = A000915(m) */

    /* Find minimal k such that k(k+1)/2 >= e4 using integer arithmetic */
    seq_t D = 1 + 8 * e4;                                      /* discriminant for quadratic */
    seq_t s = floor_sqrt(D);                                   /* floor(sqrt(D))  */
    seq_t k = (s - 1) / 2;                                      /* floor solution      */

    /* Adjust, if off‑by‑one due to the floor operation */
    seq_t tri = k * (k + 1) / 2;
    while (tri < e4) {
        ++k;
        tri = k * (k + 1) / 2;
    }

    return k;
}
