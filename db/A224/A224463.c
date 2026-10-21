/*
 * A224463  The Wiener index of the tetrathiafulvalene dendrimer defined
 *          pictorially as D[n] in the Shabani reference.
 * https://oeis.org/A224463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -64939 + 2^n*(-27280*n+344122) + 4^n*(215264*n-268088)
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);
    seq_t pow4n = pow4(n);

    /* 64‑bit signed coefficients (safe for all n used in tests) */
    int64_t c1 = 344122LL - 27280LL * (int64_t)n;          /* -27280*n + 344122 */
    int64_t c2 = 215264LL * (int64_t)n - 268088LL;        /* 215264*n - 268088 */

    seq_t pos = 0;
    seq_t neg = 0;

    if (c1 >= 0)
        pos += pow2n * (seq_t)c1;
    else
        neg += pow2n * (seq_t)(-c1);

    if (c2 >= 0)
        pos += pow4n * (seq_t)c2;
    else
        neg += pow4n * (seq_t)(-c2);

    /* constant term -64939 contributes to the negative part */
    neg += 64939;

    return pos - neg;
}
