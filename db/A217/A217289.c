/*
 * A217289  Where records occur in A217287.
 * https://oeis.org/A217289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index m where A217287(m) sets a new record
 *            (A217287(m) > A217287(k) for all k < m); computed by a
 *            sliding-window scan over m with a smallest-prime-factor sieve
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A217289_spf = 0;
static uint32_t *A217289_cnt = 0;
static uint64_t A217289_lim = 0;

static void A217289_ensure(uint64_t need)
{
    if (need <= A217289_lim) return;
    uint64_t nl = A217289_lim * 2;
    if (nl < need) nl = need;
    if (nl < 1024) nl = 1024;
    uint32_t *ns = (uint32_t *)realloc(A217289_spf, (nl + 1) * sizeof(uint32_t));
    uint32_t *nc = (uint32_t *)realloc(A217289_cnt, (nl + 1) * sizeof(uint32_t));
    A217289_spf = ns;
    A217289_cnt = nc;
    for (uint64_t i = A217289_lim + 1; i <= nl; i++) nc[i] = 0;
    if (A217289_lim == 0) nc[0] = 0;
    for (uint64_t i = 0; i <= nl; i++) ns[i] = 0;
    for (uint64_t i = 2; i <= nl; i++) {
        if (ns[i] == 0) {
            for (uint64_t j = i; j <= nl; j += i)
                if (ns[j] == 0) ns[j] = (uint32_t)i;
        }
    }
    A217289_lim = nl;
}

static void A217289_add(uint64_t m)
{
    while (m > 1) {
        uint32_t p = A217289_spf[m];
        A217289_cnt[p]++;
        while (m % p == 0) m /= p;
    }
}

static void A217289_remove(uint64_t m)
{
    while (m > 1) {
        uint32_t p = A217289_spf[m];
        A217289_cnt[p]--;
        while (m % p == 0) m /= p;
    }
}

static int A217289_has_new(uint64_t m)
{
    while (m > 1) {
        uint32_t p = A217289_spf[m];
        if (A217289_cnt[p] == 0) return 1;
        while (m % p == 0) m /= p;
    }
    return 0;
}

static uint64_t *A217289_rec = 0;
static size_t A217289_nrec = 0, A217289_cap = 0;
static uint64_t A217289_s = 1, A217289_e = 0, A217289_best = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A217289_ensure(1024);
    while (A217289_nrec < (size_t)n) {
        uint64_t s = A217289_s;
        A217289_ensure(s + 1);
        if (A217289_e < s) {
            A217289_add(s);
            A217289_e = s;
        }
        for (;;) {
            uint64_t m = A217289_e + 1;
            A217289_ensure(m);
            if (!A217289_has_new(m)) break;
            A217289_add(m);
            A217289_e = m;
        }
        uint64_t len = A217289_e - s + 1;
        if (len > A217289_best) {
            A217289_best = len;
            if (A217289_nrec == A217289_cap) {
                A217289_cap = A217289_cap ? A217289_cap * 2 : 64;
                A217289_rec = (uint64_t *)realloc(A217289_rec, A217289_cap * sizeof(uint64_t));
            }
            A217289_rec[A217289_nrec++] = s;
        }
        A217289_remove(s);
        A217289_s = s + 1;
    }
    return (seq_t)A217289_rec[n - 1];
}
