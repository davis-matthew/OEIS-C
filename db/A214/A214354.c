/*
 * A214354  Number of 4X4X4 triangular 0..n arrays with no element lying
 *          outside the (possibly reversed) range delimited by its sw and se
 *          neighbors.
 * https://oeis.org/A214354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (109/226800)n^10 + (1531/90720)n^9 + (1471/6048)n^8 +
 *            (28627/15120)n^7 + * (47321/5400)n^6 + (107767/4320)n^5 +
 *            (789913/18144)n^4 + (127789/2835)n^3 + * (111121/4200)n^2 +
 *            (5083/630)n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 177, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t SCALING = 907200;

static const uint64_t COEFFS[11] = {
    436,           /* 109/226800 * 907200 = 436     n^10 */
    15310,         /* 1531/90720  * 907200 = 15310   n^9  */
    220650,        /* 1471/6048   * 907200 = 220650  n^8  */
    1717620,       /* 28627/15120 * 907200 = 1717620 n^7  */
    7949928,       /* 47321/5400  * 907200 = 7949928 n^6  */
    22631070,      /* 107767/4320 * 907200 = 22631070 n^5 */
    39495650,      /* 789913/18144 * 907200 = 39495650 n^4 */
    40892480,      /* 127789/2835  * 907200 = 40892480 n^3 */
    24002136,      /* 111121/4200 * 907200 = 24002136 n^2 */
    7319520,       /* 5083/630    * 907200 = 7319520  n^1 */
    907200         /* constant term * 907200 = 907200  n^0 */
};

seq_t a(idx_t n)
{
    /* For small integral seq_t types, use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t pow[11];
        pow[0] = 1;
        for (idx_t i = 1; i <= 10; ++i)
            pow[i] = pow[i - 1] * (__uint128_t)n;

        __uint128_t num = 0;
        for (idx_t i = 0; i < 11; ++i)
            num += (__uint128_t)COEFFS[i] * pow[10 - i];

        return (seq_t)(num / SCALING);
    } else {
        /* For arbitrary‑precision seq_t types, rely on seq_t operator overloads. */
        seq_t pow[11];
        pow[0] = (seq_t)1;
        for (idx_t i = 1; i <= 10; ++i)
            pow[i] = pow[i - 1] * (seq_t)n;

        seq_t num = (seq_t)0;
        for (idx_t i = 0; i < 11; ++i)
            num = num + (seq_t)COEFFS[i] * pow[10 - i];

        return num / (seq_t)SCALING;
    }
}
