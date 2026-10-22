/*
 * A000471  a(n) = floor(sinh(n)).
 * https://oeis.org/A000471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(e^n/2), with e^n = Sum_{k>=0} n^k/k! summed in
 *            high-precision fixed point
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t ib = (uint64_t)n * 14427 / 10000 + 2;      /* bits of integer part of e^n */
    uint64_t K = 8 * (uint64_t)n + 100;                  /* generous bound on number of terms */
    uint64_t kb = 0;
    while (kb < 63 && ((uint64_t)1 << kb) < K) kb++;
    uint64_t fbits = ib + 64 + 2 * kb;                   /* fractional bits */
    size_t fl = (size_t)((fbits + 1 + 31) / 32);         /* scale 2^F with F+1 = 32*fl */
    size_t L = fl + (size_t)((ib + 31) / 32) + 3;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t top, i;
    uint64_t k, c, r;
    seq_t res = 0;

    t[fl - 1] = (uint32_t)1 << 31;   /* t = 2^F, the k=0 term */
    s[fl - 1] = (uint32_t)1 << 31;
    top = fl;

    for (k = 1;; k++) {
        /* t *= n */
        c = 0;
        for (i = 0; i < top; i++) {
            c += (uint64_t)t[i] * (uint64_t)n;
            t[i] = (uint32_t)c;
            c >>= 32;
        }
        while (c && top < L) {
            t[top++] = (uint32_t)c;
            c >>= 32;
        }
        /* t /= k */
        r = 0;
        for (i = top; i-- > 0;) {
            uint64_t cur = (r << 32) | t[i];
            t[i] = (uint32_t)(cur / k);
            r = cur % k;
        }
        while (top > 0 && t[top - 1] == 0) top--;
        if (top == 0) break;
        /* s += t */
        c = 0;
        for (i = 0; i < top; i++) {
            c += (uint64_t)s[i] + t[i];
            s[i] = (uint32_t)c;
            c >>= 32;
        }
        for (; c && i < L; i++) {
            c += s[i];
            s[i] = (uint32_t)c;
            c >>= 32;
        }
    }

    /* floor(S / 2^(F+1)) = limbs fl..L-1 */
    for (i = L; i-- > fl;) {
        res *= 65536;
        res *= 65536;
        res += (idx_t)s[i];
    }
    free(t);
    free(s);
    return res;
}
