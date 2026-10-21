/*
 * A214616  a(n) = n*(n+1)*(n+2)*(n+3)*(20*n^2+72*n+43)/360.
 * https://oeis.org/A214616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(20*n^2+72*n+43)/360
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* Gather the five multiplicative factors */
    seq_t f1 = sn;                 /* n */
    seq_t f2 = sn + 1;             /* n+1 */
    seq_t f3 = sn + 2;             /* n+2 */
    seq_t f4 = sn + 3;             /* n+3 */
    seq_t f5 = 20 * sn * sn + 72 * sn + 43; /* 20*n^2+72*n+43 */

    /* If the first factor is 0, the whole product is 0 */
    if (f1 == 0) return 0;

    seq_t denom = 360;   /* 360 = 2^3 * 3^2 * 5 */
    seq_t result = 1;

    /* Helper macro to process each factor */
    #define PROC_FACTOR(f)                            \
        do {                                          \
            seq_t g = gcd((f), denom);                \
            if (g > 1) {                              \
                (f) /= g;                             \
                denom /= g;                           \
            }                                         \
            result *= (f);                            \
        } while (0)

    PROC_FACTOR(f1);
    PROC_FACTOR(f2);
    PROC_FACTOR(f3);
    PROC_FACTOR(f4);
    PROC_FACTOR(f5);

    /* After cancelling with the denominator, it should be 1 */
    if (denom != 1) {
        result /= denom;
    }

    return result;
}
