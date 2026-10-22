/*
 * A006660  Number of meanders in which first bridge is 3.
 * https://oeis.org/A006660
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form. This is a transfer-matrix count of open meanders
 *            (a river enters from the south, crosses an east-west road n
 *            times, and leaves to infinity) whose first crossing is the 3rd
 *            bridge from the west. The road is scanned from west to east.
 *            Each state records the strands crossing the scan...
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, partner, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/partner.h"

typedef unsigned __int128 u128_t;

typedef struct {
    u128_t *key;
    seq_t *val;
    size_t cap, cnt;
} tab_t;

static void tab_init(tab_t *T, size_t cap) {
    T->key = (u128_t *)calloc(cap, sizeof(u128_t));
    T->val = alloc_zero((idx_t)cap);
    T->cap = cap;
    T->cnt = 0;
}

static void tab_free(tab_t *T) {
    free(T->key);
    release(T->val, (idx_t)T->cap);
}

static size_t slot_of(const tab_t *T, u128_t key) {
    uint64_t lo = (uint64_t)key, hi = (uint64_t)(key >> 64);
    uint64_t h = lo * 0x9E3779B97F4A7C15ULL ^ (hi + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    size_t m = T->cap - 1;
    size_t p = (size_t)h & m;
    while (T->key[p] != 0 && T->key[p] != key) p = (p + 1) & m;
    return p;
}

static void tab_grow(tab_t *T) {
    tab_t N2;
    size_t p;
    tab_init(&N2, T->cap * 2);
    for (p = 0; p < T->cap; p++) {
        if (T->key[p] != 0) {
            size_t q = slot_of(&N2, T->key[p]);
            N2.key[q] = T->key[p];
            N2.val[q] = T->val[p];
            N2.cnt++;
        }
    }
    tab_free(T);
    *T = N2;
}

static void tab_add(tab_t *T, u128_t key, const seq_t *v) {
    size_t p;
    if ((T->cnt + 1) * 2 > T->cap) tab_grow(T);
    p = slot_of(T, key);
    if (T->key[p] == 0) { T->key[p] = key; T->cnt++; }
    T->val[p] += *v;
}

/* symbols: 1 = '(' (partner below), 2 = ')' (partner above), 3 = S (source strand) */
static u128_t encode(const unsigned char *s, int k, int t) {
    u128_t c = 0;
    int j;
    for (j = 0; j < k; j++) c |= (u128_t)s[j] << (2 * j);
    c |= (u128_t)t << 120;
    return c;
}

static void compute_upto(seq_t *memo, int N) {
    tab_t cur, nxt;
    unsigned char s[64], ns[64];
    seq_t one = 1;
    int i;
    tab_init(&cur, 16);
    s[0] = 3;
    tab_add(&cur, encode(s, 1, 0), &one);
    for (i = 1; i <= N; i++) {
        size_t cap = 16, p;
        int maxk = 2 * (N - i) + 1;
        while (cap < cur.cnt * 4) cap <<= 1;
        tab_init(&nxt, cap);
        for (p = 0; p < cur.cap; p++) {
            u128_t c = cur.key[p];
            int t, k, b, j;
            int botOpenOk, botCloseOk;
            const seq_t *v;
            if (c == 0) continue;
            v = &cur.val[p];
            t = (int)(c >> 120);
            k = 0;
            while (k < 60 && ((c >> (2 * k)) & 3) != 0) {
                s[k] = (unsigned char)((c >> (2 * k)) & 3);
                k++;
            }
            b = k - t;
            botOpenOk = (i != 3);
            if (b < 1) botCloseOk = 0;
            else if (i < 3) botCloseOk = (s[t] != 3);
            else if (i == 3) botCloseOk = (s[t] == 3);
            else botCloseOk = 1;

            /* case 1: open top and open bottom */
            if (botOpenOk && k + 2 <= maxk) {
                for (j = 0; j < t; j++) ns[j] = s[j];
                ns[t] = 1;
                ns[t + 1] = 2;
                for (j = t; j < k; j++) ns[j + 2] = s[j];
                tab_add(&nxt, encode(ns, k + 2, t + 1), v);
            }
            /* case 2: close top, open bottom */
            if (botOpenOk && t >= 1 && k <= maxk) {
                tab_add(&nxt, encode(s, k, t - 1), v);
            }
            /* case 3: open top, close bottom */
            if (botCloseOk && k <= maxk) {
                tab_add(&nxt, encode(s, k, t + 1), v);
            }
            /* case 4: close top and close bottom */
            if (botCloseOk && t >= 1 && k - 2 <= maxk) {
                unsigned char x = s[t - 1], y = s[t];
                int m;
                if (x == 1 && y == 2) continue; /* would close a loop */
                for (j = 0; j < k; j++) ns[j] = s[j];
                if (x == 3) {
                    ns[partner(s, k, t)] = 3;
                } else if (y == 3) {
                    ns[partner(s, k, t - 1)] = 3;
                } else {
                    int px = partner(s, k, t - 1), py = partner(s, k, t);
                    int lo = px < py ? px : py, hi = px < py ? py : px;
                    ns[lo] = 1;
                    ns[hi] = 2;
                }
                m = 0;
                for (j = 0; j < k; j++) {
                    if (j == t - 1 || j == t) continue;
                    ns[m++] = ns[j];
                }
                if (m >= 1) tab_add(&nxt, encode(ns, m, t - 1), v);
            }
        }
        tab_free(&cur);
        cur = nxt;
        if (i >= 3) {
            seq_t res = 0;
            int tt;
            s[0] = 3;
            for (tt = 0; tt <= 1; tt++) {
                u128_t key = encode(s, 1, tt);
                size_t q = slot_of(&cur, key);
                if (cur.key[q] == key) res += cur.val[q];
            }
            memo[i] = res;
        }
    }
    tab_free(&cur);
}

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    if (n < 3) return 0;
    if (n < memo_len) return memo[n];
    if (n > 59) return 0;
    {
        seq_t *nm = alloc_zero(n + 1);
        idx_t i;
        compute_upto(nm, (int)n);
        if (memo != NULL) release(memo, memo_len);
        memo = nm;
        memo_len = n + 1;
        for (i = 3; i <= n; i++) { /* all of memo[3..n] filled */ }
    }
    return memo[n];
}
