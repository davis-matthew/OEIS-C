/*
 * A004576  Expansion of sqrt(7) in base 9.
 * https://oeis.org/A004576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(7) * 9^(n-1)) mod 9; computed as the integer
 *            square root of 7*81^(N-1) (multi-limb binary method), converted
 *            to base 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004576_addbit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004576_addbit.h"

static unsigned char *a004576_dig = NULL;
static idx_t a004576_ndig = 0;

static void a004576_compute(idx_t N) {
    size_t L = (size_t)((7 * N + 8) / 32 + 2);
    uint32_t *M = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t i, top, b, cnt, k;
    unsigned char *tmp;
    M[0] = 7;
    for (idx_t e = 1; e < N; e++) {
        uint64_t c = 0;
        for (i = 0; i < L; i++) {
            uint64_t v = (uint64_t)M[i] * 81 + c;
            M[i] = (uint32_t)v;
            c = v >> 32;
        }
    }
    top = L - 1;
    while (top > 0 && M[top] == 0) top--;
    b = top * 32;
    {
        uint32_t t = M[top];
        int hb = 31;
        while (hb > 0 && !((t >> hb) & 1u)) hb--;
        b += (size_t)hb;
    }
    b &= ~(size_t)1;
    for (;;) {
        int ge = 1;
        memcpy(T, R, L * sizeof(uint32_t));
        a004576_addbit(T, L, b);
        for (i = L; i-- > 0;) {
            if (M[i] != T[i]) { ge = M[i] > T[i]; break; }
        }
        {
            /* R >>= 1 */
            for (i = 0; i < L; i++) {
                uint32_t hi = (i + 1 < L) ? R[i + 1] : 0;
                R[i] = (R[i] >> 1) | (hi << 31);
            }
        }
        if (ge) {
            int64_t br = 0;
            for (i = 0; i < L; i++) {
                int64_t d = (int64_t)M[i] - (int64_t)T[i] - br;
                if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
                M[i] = (uint32_t)d;
            }
            a004576_addbit(R, L, b);
        }
        if (b == 0) break;
        b -= 2;
    }
    /* convert R to base 9, least significant first */
    tmp = (unsigned char *)malloc((size_t)N + 64);
    cnt = 0;
    top = L;
    while (top > 0 && R[top - 1] == 0) top--;
    while (top > 0) {
        uint64_t r = 0;
        for (i = top; i-- > 0;) {
            uint64_t cur = (r << 32) | R[i];
            R[i] = (uint32_t)(cur / 387420489u);
            r = cur % 387420489u;
        }
        for (k = 0; k < 9; k++) {
            if (cnt < (size_t)N + 64) tmp[cnt++] = (unsigned char)(r % 9);
            r /= 9;
        }
        while (top > 0 && R[top - 1] == 0) top--;
    }
    while (cnt > 0 && tmp[cnt - 1] == 0) cnt--;
    free(a004576_dig);
    a004576_dig = (unsigned char *)malloc(cnt + 1);
    for (k = 0; k < cnt; k++) a004576_dig[k] = tmp[cnt - 1 - k];
    a004576_ndig = (idx_t)cnt;
    free(tmp);
    free(M);
    free(R);
    free(T);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a004576_ndig) {
        idx_t N = n + n / 4 + 16;
        a004576_compute(N);
    }
    return (seq_t)a004576_dig[n - 1];
}
