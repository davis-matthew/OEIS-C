/*
 * A007806  Integer part of Sum_{i=1..n} binomial(n,i) * (n/i)^i.
 * https://oeis.org/A007806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Sum_{i=1..n} binomial(n,i) * (n/i)^i), computed by
 *            exact multi-limb fixed-point arithmetic; guard bits are
 *            increased until the floor is certain
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   a7806_add, a7806_div, a7806_gcd, a7806_mul, a7806_powop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a7806_add.h"
#include "../utils/a7806_div.h"
#include "../utils/a7806_gcd.h"
#include "../utils/a7806_mul.h"
#include "../utils/a7806_powop.h"

seq_t a(idx_t n) {
    uint64_t bl = 0, t;
    size_t w = 3;
    for (t = n; t; t >>= 1) bl++;
    for (;;) {
        size_t cap = (size_t)((n + n * bl) / 32) + w + 8;
        uint32_t *B = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *T = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *S = (uint32_t *)calloc(cap + 1, sizeof(uint32_t));
        size_t blen = 1, slen = 0, tlen;
        uint64_t cnt = 0, c;
        idx_t i;
        size_t j;
        int ambiguous;
        B[0] = 1;
        for (i = 1; i <= n; i++) {
            uint64_t d, np, ip;
            a7806_mul(B, &blen, (uint32_t)(n - i + 1));
            a7806_div(B, &blen, (uint32_t)i);
            memset(T, 0, w * sizeof(uint32_t));
            memcpy(T + w, B, blen * sizeof(uint32_t));
            tlen = w + blen;
            d = a7806_gcd(n, i);
            np = n / d;
            ip = i / d;
            if (np > 1) a7806_powop(T, &tlen, np, i, 0);
            if (ip > 1) cnt += (uint64_t)a7806_powop(T, &tlen, ip, i, 1);
            a7806_add(S, &slen, T, tlen);
        }
        /* true sum lies in [S, S + cnt) / 2^(32w); check the floor is the same at both ends */
        c = cnt;
        for (j = 0; j < w; j++) {
            uint64_t limb = j < slen ? S[j] : 0;
            c = (limb + c) >> 32;
        }
        ambiguous = (c != 0);
        if (!ambiguous) {
            seq_t r = 0;
            for (j = slen; j > w; j--) {
                uint32_t limb = S[j - 1];
                r = (seq_t)(r * 65536u + (limb >> 16));
                r = (seq_t)(r * 65536u + (limb & 0xFFFFu));
            }
            free(B); free(T); free(S);
            return r;
        }
        free(B); free(T); free(S);
        w *= 2;
    }
}
