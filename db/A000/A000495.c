/*
 * A000495  Nearest integer to sinh(n).
 * https://oeis.org/A000495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sinh(n)) = round(Sum_{k odd} n^k/k!), summed
 *            exactly in binary fixed point with enough guard bits
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t P = nn * 1443 / 1000 + 110;           /* fractional bits */
    size_t L = (size_t)((P + nn * 1443 / 1000 + 96) / 32 + 2);
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t top, i, w, m, j;
    unsigned b;
    uint64_t k, c;
    seq_t r = 0;

    T[P / 32] = (uint32_t)1 << (P % 32);           /* T = 2^P * n^0/0! */
    top = (size_t)(P / 32 + 1);
    for (k = 1;; k++) {
        /* T *= n */
        c = 0;
        for (i = 0; i < top; i++) {
            c += (uint64_t)T[i] * nn;
            T[i] = (uint32_t)c;
            c >>= 32;
        }
        while (c && top < L) {
            T[top++] = (uint32_t)c;
            c >>= 32;
        }
        /* T /= k */
        {
            uint64_t rem = 0;
            for (i = top; i-- > 0;) {
                uint64_t cur = (rem << 32) | T[i];
                T[i] = (uint32_t)(cur / k);
                rem = cur % k;
            }
        }
        while (top > 0 && T[top - 1] == 0) top--;
        if (top == 0) break;
        if (k & 1) {
            c = 0;
            for (i = 0; i < top; i++) {
                c += (uint64_t)S[i] + T[i];
                S[i] = (uint32_t)c;
                c >>= 32;
            }
            while (c && i < L) {
                c += S[i];
                S[i] = (uint32_t)c;
                c >>= 32;
                i++;
            }
        }
    }

    /* add 1/2 for rounding */
    {
        size_t idx = (size_t)((P - 1) / 32);
        c = (uint64_t)S[idx] + ((uint64_t)1 << ((P - 1) % 32));
        S[idx] = (uint32_t)c;
        c >>= 32;
        idx++;
        while (c && idx < L) {
            c += S[idx];
            S[idx] = (uint32_t)c;
            c >>= 32;
            idx++;
        }
    }

    /* result = S >> P */
    w = (size_t)(P / 32);
    b = (unsigned)(P % 32);
    m = L - w;
    for (j = m; j-- > 0;) {
        uint32_t lo = S[w + j] >> b;
        uint32_t hi = 0;
        uint32_t limb;
        if (b && w + j + 1 < L) hi = S[w + j + 1] << (32 - b);
        limb = lo | hi;
        r = (seq_t)(r * 256);
        r = (seq_t)(r * 256);
        r = (seq_t)(r + (seq_t)(limb >> 16));
        r = (seq_t)(r * 256);
        r = (seq_t)(r * 256);
        r = (seq_t)(r + (seq_t)(limb & 0xFFFFu));
    }
    free(T);
    free(S);
    return r;
}
