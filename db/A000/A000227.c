/*
 * A000227  Nearest integer to e^n.
 * https://oeis.org/A000227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = round(e^n), where e^n = Sum_{k>=0} n^k/k! is evaluated by
 *            Horner's rule (x = 1 + n/k * x for k = K..1) in binary fixed
 *            point with enough guard bits
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t y = (N * 1443) / 1000;           /* >= log2(e) * n */
    size_t P = (size_t)((y + 64) / 32 + 1);    /* fractional limbs */
    size_t I = (size_t)(y / 32 + 2);           /* integer limbs */
    size_t L = P + I + 2;
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t K = 3 * N + 200, k;
    size_t len, i;
    seq_t r;

    x[P] = 1;
    len = P + 1;
    for (k = K; k >= 1; k--) {
        uint64_t c = 0, rem = 0;
        /* x *= n */
        for (i = 0; i < len; i++) {
            uint64_t t = (uint64_t)x[i] * N + c;
            x[i] = (uint32_t)t;
            c = t >> 32;
        }
        while (c) {
            x[len++] = (uint32_t)c;
            c >>= 32;
        }
        /* x /= k */
        for (i = len; i-- > 0;) {
            uint64_t t = (rem << 32) | x[i];
            x[i] = (uint32_t)(t / k);
            rem = t % k;
        }
        while (len > 0 && x[len - 1] == 0)
            len--;
        /* x += 1 */
        i = P;
        for (;;) {
            if (++x[i] != 0)
                break;
            i++;
        }
        if (i + 1 > len)
            len = i + 1;
    }

    /* round: add 1/2 */
    i = P - 1;
    {
        uint64_t t = (uint64_t)x[i] + 0x80000000u;
        x[i] = (uint32_t)t;
        if (t >> 32) {
            i++;
            for (;;) {
                if (++x[i] != 0)
                    break;
                i++;
            }
            if (i + 1 > len)
                len = i + 1;
        }
    }

    r = 0;
    for (i = len; i-- > P;) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            unsigned b = (unsigned)((x[i] >> s) & 0xFFu);
            r = r * 256;
            r = r + (seq_t)b;
        }
    }
    free(x);
    return r;
}
