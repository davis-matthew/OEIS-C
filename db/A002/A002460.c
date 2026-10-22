/*
 * A002460  Nearest integer to exponential integral of n.
 * https://oeis.org/A002460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = round(Ei(n)), with Ei(n) = gamma + log(n) + Sum_{k>=1}
 *            n^k/(k*k!) and log(n) = e*log(2) + 2*atanh((n-2^e)/(n+2^e)),
 *            log(2) = 2*atanh(1/3); evaluated in exact multi-limb fixed-point
 *            arithmetic
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 93,
 *            big any n
 * Helpers:   ei_addto, ei_atanh, ei_div, ei_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ei_addto.h"
#include "../utils/ei_atanh.h"
#include "../utils/ei_div.h"
#include "../utils/ei_mul.h"

seq_t a(idx_t n)
{
    static const char *gdig =
        "577215664901532860606512090082402431042159335939923598805767";
    uint64_t nn = (uint64_t)n;
    size_t FL = (size_t)((nn * 14427 / 10000 + 128) / 32 + 1);
    size_t IL = (size_t)((nn * 14427 / 10000 + 64) / 32 + 2);
    size_t L = FL + IL;
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t tl, ul, i;
    uint32_t k, e;
    uint64_t pw;
    seq_t r = 0;

    /* series Sum_{k>=1} n^k/(k*k!) */
    t[FL] = 1;
    tl = FL + 1;
    for (k = 1; tl > 0; k++) {
        ei_mul(t, &tl, L, (uint32_t)nn);
        ei_div(t, &tl, k);
        if (tl == 0) break;
        memcpy(u, t, tl * sizeof(uint32_t));
        ul = tl;
        ei_div(u, &ul, k);
        ei_addto(s, L, u, ul);
    }

    /* log(n) */
    e = 0;
    pw = 1;
    while (pw * 2 <= nn) { pw *= 2; e++; }
    ei_atanh(s, L, FL, e, 1, 3, t, u);
    ei_atanh(s, L, FL, 1, (uint32_t)(nn - pw), (uint32_t)(nn + pw), t, u);

    /* gamma */
    memset(t, 0, L * sizeof(uint32_t));
    {
        size_t len = strlen(gdig);
        for (i = len; i-- > 0;) {
            t[FL] += (uint32_t)(gdig[i] - '0');
            tl = FL + 1;
            ei_div(t, &tl, 10);
        }
        tl = FL + 1;
        while (tl > 0 && t[tl - 1] == 0) tl--;
        ei_addto(s, L, t, tl);
    }

    /* round to nearest */
    memset(t, 0, L * sizeof(uint32_t));
    t[FL - 1] = 0x80000000u;
    ei_addto(s, L, t, FL);

    for (i = L; i-- > FL;) {
        r *= 65536;
        r *= 65536;
        r += (seq_t)s[i];
    }
    free(s);
    free(t);
    free(u);
    return r;
}
