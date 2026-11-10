/*
 * A077699  a(n) = A077698(n+1)/A077698(n).
 * https://oeis.org/A077699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A077698(n+1)/A077698(n), where A077698(1) = 3 and
 *            A077698(n+1) is the smallest multiple k*A077698(n), k >= 2, not
 *            divisible by 10, whose decimal digits include every digit of
 *            A077698(n) at least as often. Computed by search: step through
 *            the multiples by repeated addition on an internal ba...
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A077699_P = NULL;   /* current A077698 term, base 1000, little endian */
static size_t    A077699_plen = 0;
static uint32_t *A077699_C = NULL;   /* candidate multiple */
static size_t    A077699_clen = 0;
static size_t    A077699_cap = 0;    /* capacity of P and C (limbs) */
static uint64_t *A077699_r = NULL;   /* computed ratios */
static size_t    A077699_rcnt = 0;
static size_t    A077699_rcap = 0;
static unsigned char A077699_dig[1000][3];
static bool      A077699_init = false;

static void A077699_ensure(size_t need)
{
    if (need <= A077699_cap)
        return;
    size_t nc = A077699_cap ? A077699_cap : 64;
    while (nc < need)
        nc *= 2;
    A077699_P = (uint32_t *)realloc(A077699_P, nc * sizeof(uint32_t));
    A077699_C = (uint32_t *)realloc(A077699_C, nc * sizeof(uint32_t));
    A077699_cap = nc;
}

static void A077699_count(const uint32_t *x, size_t len, unsigned int cnt[10])
{
    for (int d = 0; d < 10; ++d)
        cnt[d] = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        const unsigned char *g = A077699_dig[x[i]];
        cnt[g[0]]++; cnt[g[1]]++; cnt[g[2]]++;
    }
    uint32_t top = x[len - 1];
    do {
        cnt[top % 10]++;
        top /= 10;
    } while (top);
}

static void A077699_next(void)
{
    unsigned int pf[10], cf[10];
    A077699_count(A077699_P, A077699_plen, pf);

    A077699_ensure(A077699_plen + 2);
    memcpy(A077699_C, A077699_P, A077699_plen * sizeof(uint32_t));
    A077699_clen = A077699_plen;
    uint64_t k = 1;

    for (;;) {
        /* C += P */
        A077699_ensure(A077699_clen + 2);
        uint32_t carry = 0;
        size_t i = 0;
        for (; i < A077699_plen; ++i) {
            uint32_t s = A077699_C[i] + A077699_P[i] + carry;
            if (s >= 1000) { s -= 1000; carry = 1; } else carry = 0;
            A077699_C[i] = s;
        }
        while (carry) {
            if (i == A077699_clen) {
                A077699_C[A077699_clen++] = 0;
            }
            uint32_t s = A077699_C[i] + carry;
            if (s >= 1000) { s -= 1000; carry = 1; } else carry = 0;
            A077699_C[i] = s;
            ++i;
        }
        ++k;

        if (A077699_C[0] % 10 == 0)
            continue;
        A077699_count(A077699_C, A077699_clen, cf);
        bool ok = true;
        for (int d = 0; d < 10; ++d)
            if (cf[d] < pf[d]) { ok = false; break; }
        if (ok)
            break;
    }

    if (A077699_rcnt >= A077699_rcap) {
        A077699_rcap = A077699_rcap ? A077699_rcap * 2 : 64;
        A077699_r = (uint64_t *)realloc(A077699_r, A077699_rcap * sizeof(uint64_t));
    }
    A077699_r[A077699_rcnt++] = k;

    memcpy(A077699_P, A077699_C, A077699_clen * sizeof(uint32_t));
    A077699_plen = A077699_clen;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (!A077699_init) {
        for (int v = 0; v < 1000; ++v) {
            A077699_dig[v][0] = (unsigned char)(v % 10);
            A077699_dig[v][1] = (unsigned char)((v / 10) % 10);
            A077699_dig[v][2] = (unsigned char)(v / 100);
        }
        A077699_ensure(64);
        A077699_P[0] = 3;
        A077699_plen = 1;
        A077699_init = true;
    }
    while ((idx_t)A077699_rcnt < n)
        A077699_next();
    return (seq_t)A077699_r[n - 1];
}
