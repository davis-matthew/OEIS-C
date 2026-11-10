/*
 * A079290  Composite numbers satisfying A073078(n)=(n+1)/2.
 * https://oeis.org/A079290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: odd composite m such that no k < (m+1)/2 has m |
 *            binomial(2k,k), i.e. A073078(m) = (m+1)/2
 * Range:     u8 n <= 10, u16 n <= 79, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: standard, pass, 59 terms through n = 93 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* v_p(C(2k,k)) via Legendre / Kummer */
static uint64_t A079290_vp_central(uint64_t k, uint64_t p)
{
    uint64_t got = 0, twok = k + k, pk = p;
    while (pk <= twok) {
        got += (twok / pk) - 2 * (k / pk);
        if (pk > UINT64_MAX / p) break;
        pk *= p;
    }
    return got;
}

static bool A079290_is_term(uint64_t m)
{
    uint64_t pr[64], ex[64], pp[64];
    int cnt = 0;
    uint64_t r = m;
    for (uint64_t d = 3; d * d <= r; d += 2) {
        if (r % d == 0) {
            pr[cnt] = d; ex[cnt] = 0; pp[cnt] = 1;
            while (r % d == 0) { r /= d; ex[cnt]++; pp[cnt] *= d; }
            cnt++;
        }
    }
    if (r > 1) { pr[cnt] = r; ex[cnt] = 1; pp[cnt] = r; cnt++; }

    if (cnt == 1 && ex[0] == 1) return false;   /* prime */
    /* odd prime power p^e: minimal k is exactly (p^e+1)/2 */
    if (cnt == 1) return true;

    /* minimal k is at least (Q+1)/2 for every prime power Q || m */
    uint64_t Q = 0;
    for (int i = 0; i < cnt; i++) if (pp[i] > Q) Q = pp[i];

    uint64_t half = (m + 1) / 2;
    for (uint64_t k = (Q + 1) / 2; k < half; k++) {
        bool ok = true;
        for (int i = 0; i < cnt && ok; i++)
            if (A079290_vp_central(k, pr[i]) < ex[i]) ok = false;
        if (ok) return false;
    }
    /* check that k = (m+1)/2 works */
    for (int i = 0; i < cnt; i++)
        if (A079290_vp_central(half, pr[i]) < ex[i]) return false;
    return true;
}

static uint64_t *A079290_terms = NULL;
static size_t A079290_cnt = 0, A079290_cap = 0;
static uint64_t A079290_next_m = 9;

seq_t a(idx_t n)
{
    while (A079290_cnt < n) {
        uint64_t m = A079290_next_m;
        A079290_next_m += 2;
        if (A079290_is_term(m)) {
            if (A079290_cnt == A079290_cap) {
                size_t nc = A079290_cap ? A079290_cap * 2 : 64;
                A079290_terms = mem_resize(uint64_t, A079290_terms, nc);
                A079290_cap = nc;
            }
            A079290_terms[A079290_cnt++] = m;
        }
    }
    return (seq_t)A079290_terms[n - 1];
}
