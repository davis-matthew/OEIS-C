/*
 * A002195  Numerators of coefficients for numerical integration.
 * https://oeis.org/A002195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of (2/(2n+1)!)*Integral_{t=0..1}
 *            t*Product_{k=1..n} (t^2-k^2) = numerator of (1/(2n+1)!) *
 *            Sum_{j=0..n} [u^j]Product_{k=1..n}(u-k^2) / (j+1) (substituting
 *            u = t^2; exact big-integer arithmetic, reduced by primes <=
 *            2n+1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   p2195_addmul, p2195_bitlen, p2195_cmp, p2195_divsmall,
 *            p2195_muladd, p2195_norm, p2195_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/p2195_addmul.h"
#include "../utils/p2195_bitlen.h"
#include "../utils/p2195_cmp.h"
#include "../utils/p2195_divsmall.h"
#include "../utils/p2195_muladd.h"
#include "../utils/p2195_norm.h"
#include "../utils/p2195_sub.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n, k, j, CP, CL, CA;
    uint64_t bitsP = 1;
    uint32_t *c, *Lb, *q, *pos, *neg, *r, *tmp;
    size_t *clen, Ll, ql, posl = 0, negl = 0, rl;
    int negative = 0;
    char *comp;
    size_t M = 2 * N + 1, p;
    seq_t res = 0;

    for (k = 1; k <= N; k++) bitsP += (uint64_t)p2195_bitlen((uint64_t)k * k + 1);
    CP = (size_t)(bitsP / 32) + 2;
    CL = (2 * (N + 1)) / 32 + 4;
    CA = CP + CL + 4;

    /* coefficients of Product_{k=1..n} (u + k^2) */
    c = (uint32_t *)calloc((N + 1) * CP, sizeof(uint32_t));
    clen = (size_t *)calloc(N + 1, sizeof(size_t));
    c[0] = 1; clen[0] = 1;
    for (k = 1; k <= N; k++) {
        uint32_t s = (uint32_t)(k * k);
        for (j = k; j >= 1; j--)
            p2195_muladd(c + j * CP, &clen[j], s, c + (j - 1) * CP, clen[j - 1]);
        p2195_muladd(c, &clen[0], s, NULL, 0);
    }

    /* primes up to 2n+1 */
    comp = (char *)calloc(M + 2, 1);
    for (p = 2; p * p <= M; p++)
        if (!comp[p])
            for (k = p * p; k <= M; k += p) comp[k] = 1;

    /* L = lcm(1..n+1) */
    Lb = (uint32_t *)calloc(CL, sizeof(uint32_t));
    Lb[0] = 1; Ll = 1;
    for (p = 2; p <= N + 1; p++) {
        size_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= N + 1; pk *= p) {
            p2195_muladd(Lb, &Ll, (uint32_t)p, NULL, 0);
            if (pk > (N + 1) / p) break;
        }
    }

    q = (uint32_t *)calloc(CL, sizeof(uint32_t));
    pos = (uint32_t *)calloc(CA, sizeof(uint32_t));
    neg = (uint32_t *)calloc(CA, sizeof(uint32_t));
    for (j = 0; j <= N; j++) {
        memset(q, 0, CL * sizeof(uint32_t));
        p2195_divsmall(Lb, Ll, (uint32_t)(j + 1), q);
        ql = p2195_norm(q, Ll);
        if (((N - j) & 1) == 0)
            p2195_addmul(pos, &posl, CA, c + j * CP, clen[j], q, ql);
        else
            p2195_addmul(neg, &negl, CA, c + j * CP, clen[j], q, ql);
    }

    if (p2195_cmp(pos, posl, neg, negl) >= 0) {
        p2195_sub(pos, &posl, neg, negl);
        r = pos; rl = posl;
    } else {
        p2195_sub(neg, &negl, pos, posl);
        r = neg; rl = negl;
        negative = 1;
    }

    /* divide out gcd with L*(2n+1)! */
    tmp = (uint32_t *)calloc(CA, sizeof(uint32_t));
    for (p = 2; p <= M && rl > 0; p++) {
        size_t e = 0, t;
        if (comp[p]) continue;
        for (t = M / p; t > 0; t /= p) e += t;
        for (t = p; t <= N + 1; t *= p) {
            e++;
            if (t > (N + 1) / p) break;
        }
        while (e > 0) {
            if (p2195_divsmall(r, rl, (uint32_t)p, NULL) != 0) break;
            memset(tmp, 0, CA * sizeof(uint32_t));
            p2195_divsmall(r, rl, (uint32_t)p, tmp);
            memcpy(r, tmp, rl * sizeof(uint32_t));
            rl = p2195_norm(r, rl);
            e--;
        }
    }

    for (k = rl; k > 0; k--) {
        uint32_t x = r[k - 1];
        res = res * 65536 + (idx_t)(x >> 16);
        res = res * 65536 + (idx_t)(x & 0xffffu);
    }
    if (negative) res = -res;

    free(c); free(clen); free(comp); free(Lb); free(q);
    free(pos); free(neg); free(tmp);
    return res;
}
