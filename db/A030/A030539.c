/*
 * A030539  (2^n+1)*(2^n+2)*(2^n+4)*(2^n+6)*(4^n+15*2^n+176)/8!.
 * https://oeis.org/A030539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n+1)*(2^n+2)*(2^n+4)*(2^n+6)*(4^n+15*2^n+176)/8!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 8! = 40320 = 2^7·3^2·5·7 */

#include "../utils/pow2.h"

/* small power for base=2,3,5,7 (exponent is tiny) */
static inline seq_t pow_int(seq_t base, unsigned int exp)
{
    seq_t r = 1;
    while (exp--) r *= base;
    return r;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    /* base powers */
    seq_t pow2n = pow2(n);            /* 2^n */
    seq_t pow4n = pow2n * pow2n;      /* 4^n */

    /* the five factors of the numerator */
    seq_t f[5];
    f[0] = pow2n + 1;                 /* 2^n + 1 */
    f[1] = pow2n + 2;                 /* 2^n + 2 */
    f[2] = pow2n + 4;                 /* 2^n + 4 */
    f[3] = pow2n + 6;                 /* 2^n + 6 */
    f[4] = pow4n + 15 * pow2n + 176;  /* 4^n + 15·2^n + 176 */

    /* exponent counters for 2,3,5,7 in all factors */
    unsigned int total2 = 0, total3 = 0, total5 = 0, total7 = 0;
    seq_t r[5];

    /* factor out 2,3,5,7 from each factor */
    for (int i = 0; i < 5; ++i) {
        seq_t tmp = f[i];
        unsigned int c2 = 0, c3 = 0, c5 = 0, c7 = 0;

        /* factor 2 by bit test */
        while ((tmp & 1) == 0) {      /* tmp % 2 == 0 */
            tmp >>= 1;
            ++c2;
        }
        while (tmp % 3 == 0) { tmp /= 3; ++c3; }
        while (tmp % 5 == 0) { tmp /= 5; ++c5; }
        while (tmp % 7 == 0) { tmp /= 7; ++c7; }

        r[i] = tmp;                    /* remaining part */
        total2 += c2; total3 += c3; total5 += c5; total7 += c7;
    }

    /* how many of the denominator primes are still needed */
    int rem2 = (int)total2 - 7;          /* 2^7 in 8! */
    int rem3 = (int)total3 - 2;          /* 3^2 */
    int rem5 = (int)total5 - 1;          /* 5   */
    int rem7 = (int)total7 - 1;          /* 7   */

    /* guard against unexpected negative remainders (should not occur) */
    if (rem2 < 0) rem2 = 0;
    if (rem3 < 0) rem3 = 0;
    if (rem5 < 0) rem5 = 0;
    if (rem7 < 0) rem7 = 0;

    /* sort r[] ascending to keep intermediate product small */
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            if (r[j] < r[i]) {
                seq_t tmp = r[i];
                r[i] = r[j];
                r[j] = tmp;
            }
        }
    }

    /* product of the leftover factors (no 2,3,5,7) */
    seq_t prod = 1;
    for (int i = 0; i < 5; ++i) prod *= r[i];

    /* multiply the remaining denom primes (small powers) */
    if (rem2 > 0) prod *= pow_int((seq_t)2, (unsigned)rem2);
    if (rem3 > 0) prod *= pow_int((seq_t)3, (unsigned)rem3);
    if (rem5 > 0) prod *= pow_int((seq_t)5, (unsigned)rem5);
    if (rem7 > 0) prod *= pow_int((seq_t)7, (unsigned)rem7);

    return prod;
}
