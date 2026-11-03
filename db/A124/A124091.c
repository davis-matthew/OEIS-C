/*
 * A124091  Decimal expansion of Fibonacci binary constant: Sum{i>=0}
 *          (1/2)^Fibonacci(i).
 * https://oeis.org/A124091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digit of the constant Σ 2^{-Fibonacci(i)}
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9891 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124091: the decimal digits of Sum_{i>=0} 2^-F(i) (F the Fibonacci numbers),
 * a(1) being the integer part (2). The sum is held as a binary fraction of M
 * bits: N = Sum 2^(M - F(i)) over F(i) <= M, the tail being below 2^(F - M) for
 * the first F(i) > M, far under the last digit kept. Digits come from
 * repeatedly multiplying the fraction by 10. M is chosen for the digit asked
 * for (the original fixed it for 10000 digits) and doubles when needed. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *digits = NULL;      /* digits[0] = integer part */
static size_t ndig = 0;                   /* digits kept from the last build */

static void build(size_t want)            /* digits[0..want] */
{
    size_t M = (size_t)(want * 3.3219280948873623) + 256;     /* bits */
    size_t W = M / 32 + 2;
    uint32_t *R = (uint32_t *)calloc(W + 1, sizeof *R);
    unsigned char *d = (unsigned char *)malloc(want + 1);
    if (!R || !d) abort();
    uint64_t f0 = 0, f1 = 1;
    unsigned ip = 0;                      /* the integer part: F = 0 and two F = 1 */
    for (;;) {
        if (f0 <= M) {                    /* add 2^(M - f0) */
            if (f0 == 0) ++ip;
            else {
                size_t k = M - f0, w = k / 32;
                uint64_t c = (uint64_t)1 << (k % 32);
                for (size_t i = w; c && i <= W; ++i) {
                    c += R[i];
                    R[i] = (uint32_t)c;
                    c >>= 32;
                }
            }
        } else break;
        uint64_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* bits >= M of R belong to the integer part */
    const size_t top = M / 32;
    const unsigned sh = (unsigned)(M % 32);
    unsigned extra = (unsigned)((((uint64_t)R[top + 1] << 32) | R[top]) >> sh);
    d[0] = (unsigned char)(ip + extra);
    R[top] &= (uint32_t)((UINT64_C(1) << sh) - 1);
    R[top + 1] = 0;
    for (size_t j = 1; j <= want; ++j) {
        uint64_t c = 0;
        for (size_t i = 0; i <= top + 1; ++i) {
            c += (uint64_t)R[i] * 10;
            R[i] = (uint32_t)c;
            c >>= 32;
        }
        unsigned dig = (unsigned)((((uint64_t)R[top + 1] << 32) | R[top]) >> sh);
        d[j] = (unsigned char)dig;
        R[top] &= (uint32_t)((UINT64_C(1) << sh) - 1);
        R[top + 1] = 0;
    }
    free(R);
    free(digits);
    digits = d;
    ndig = want + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > ndig) {
        size_t want = ndig ? 2 * ndig : 1024;
        while (want < (size_t)n) want *= 2;
        build(want);
    }
    return (seq_t)digits[n - 1];
}
