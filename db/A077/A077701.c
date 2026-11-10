/*
 * A077701  a(n) = A077700(n+1)/A077700(n).
 * https://oeis.org/A077701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A077700(n+1)/A077700(n), where A077700(1) = 4 and
 *            A077700(n+1) is the smallest multiple of A077700(n) that is not
 *            a multiple of 10 and contains every digit of A077700(n) at least
 *            as many times. Computed by search: the multiplier k = 2, 3, ...
 *            is tried for each step, and the running term is...
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Packed digit counts: digits 0..4 in word 0, digits 5..9 in word 1,
   12-bit fields (11 value bits + 1 guard bit). */
static uint64_t A077701_tabF[10000][2]; /* all 4 digits of a limb, leading zeros included */
static uint64_t A077701_tabT[10000][2]; /* top limb, leading zeros not counted */
static bool A077701_tabs_ready = false;

static void A077701_add_digit(uint64_t w[2], unsigned d)
{
    if (d < 5) w[0] += (uint64_t)1 << (12 * d);
    else       w[1] += (uint64_t)1 << (12 * (d - 5));
}

static void A077701_init_tabs(void)
{
    for (unsigned v = 0; v < 10000; ++v) {
        uint64_t f[2] = {0, 0}, t[2] = {0, 0};
        unsigned x = v;
        for (int j = 0; j < 4; ++j) { A077701_add_digit(f, x % 10); x /= 10; }
        x = v;
        do { A077701_add_digit(t, x % 10); x /= 10; } while (x);
        A077701_tabF[v][0] = f[0]; A077701_tabF[v][1] = f[1];
        A077701_tabT[v][0] = t[0]; A077701_tabT[v][1] = t[1];
    }
    A077701_tabs_ready = true;
}

static void A077701_count(const uint32_t *l, size_t len, uint64_t *lo, uint64_t *hi)
{
    uint64_t a0 = 0, a1 = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        a0 += A077701_tabF[l[i]][0];
        a1 += A077701_tabF[l[i]][1];
    }
    a0 += A077701_tabT[l[len - 1]][0];
    a1 += A077701_tabT[l[len - 1]][1];
    *lo = a0; *hi = a1;
}

/* Memoized state */
static uint64_t *A077701_ks = NULL;   /* ks[i] = a(i+1) */
static size_t A077701_nk = 0, A077701_kcap = 0;
static uint32_t *A077701_cur = NULL;  /* A077700(nk+1), base 10^4, little-endian */
static size_t A077701_curlen = 0;

static void A077701_step(void)
{
    const uint64_t G = ((uint64_t)1 << 11) | ((uint64_t)1 << 23) | ((uint64_t)1 << 35)
                     | ((uint64_t)1 << 47) | ((uint64_t)1 << 59);
    const uint32_t *p = A077701_cur;
    size_t pl = A077701_curlen;
    uint64_t plo, phi;
    A077701_count(p, pl, &plo, &phi);

    size_t cap = pl + 12;
    uint32_t *cand = (uint32_t *)malloc(cap * sizeof(uint32_t));
    memcpy(cand, p, pl * sizeof(uint32_t));
    size_t cl = pl;
    uint64_t k = 1;

    for (;;) {
        ++k;
        /* cand += p */
        uint32_t carry = 0;
        size_t i = 0;
        for (; i < pl; ++i) {
            uint32_t s = cand[i] + p[i] + carry;
            if (s >= 10000) { s -= 10000; carry = 1; } else carry = 0;
            cand[i] = s;
        }
        for (; carry && i < cl; ++i) {
            uint32_t s = cand[i] + 1;
            if (s >= 10000) { s -= 10000; carry = 1; } else carry = 0;
            cand[i] = s;
        }
        if (carry) {
            if (cl == cap) {
                cap *= 2;
                cand = (uint32_t *)realloc(cand, cap * sizeof(uint32_t));
            }
            cand[cl++] = 1;
        }

        if (cand[0] % 10 == 0) continue;

        uint64_t clo, chi;
        A077701_count(cand, cl, &clo, &chi);
        if ((((clo | G) - plo) & G) != G) continue;
        if ((((chi | G) - phi) & G) != G) continue;
        break;
    }

    free(A077701_cur);
    A077701_cur = cand;
    A077701_curlen = cl;

    if (A077701_nk == A077701_kcap) {
        A077701_kcap = A077701_kcap ? 2 * A077701_kcap : 64;
        A077701_ks = (uint64_t *)realloc(A077701_ks, A077701_kcap * sizeof(uint64_t));
    }
    A077701_ks[A077701_nk++] = k;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A077701_tabs_ready) A077701_init_tabs();
    if (A077701_cur == NULL) {
        A077701_cur = (uint32_t *)malloc(sizeof(uint32_t));
        A077701_cur[0] = 4;
        A077701_curlen = 1;
    }
    while ((idx_t)A077701_nk < n) A077701_step();
    return (seq_t)A077701_ks[n - 1];
}
