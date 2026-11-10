/*
 * A050091  Numbers k such that A050088(k) > A050088(k+1).
 * https://oeis.org/A050091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A050088(k) > A050088(k+1); computed by exact
 *            big-number simulation of A050088 (floor(x/3) if positive and
 *            unseen, else 6x), hashing seen values, recording descents
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t **A050091_T = NULL;
static size_t *A050091_L = NULL;
static uint64_t *A050091_H = NULL;
static size_t A050091_cnt = 0, A050091_cap = 0;
static size_t *A050091_tab = NULL;
static size_t A050091_tabcap = 0;
static size_t *A050091_D = NULL;
static size_t A050091_dcnt = 0, A050091_dcap = 0;

static uint64_t A050091_hash(const uint32_t *p, size_t len)
{
    uint64_t h = 1469598103934665603ULL ^ (uint64_t)len;
    size_t i;
    for (i = 0; i < len; i++) {
        h ^= p[i];
        h *= 1099511628211ULL;
        h ^= h >> 31;
    }
    return h;
}

static bool A050091_find(const uint32_t *p, size_t len, uint64_t h)
{
    size_t mask = A050091_tabcap - 1;
    size_t i = (size_t)h & mask;
    while (A050091_tab[i]) {
        size_t idx = A050091_tab[i] - 1;
        if (A050091_H[idx] == h && A050091_L[idx] == len &&
            memcmp(A050091_T[idx], p, len * sizeof(uint32_t)) == 0)
            return true;
        i = (i + 1) & mask;
    }
    return false;
}

static void A050091_tab_insert(size_t idx)
{
    size_t mask = A050091_tabcap - 1;
    size_t i = (size_t)A050091_H[idx] & mask;
    while (A050091_tab[i]) i = (i + 1) & mask;
    A050091_tab[i] = idx + 1;
}

static void A050091_add(uint32_t *p, size_t len, uint64_t h)
{
    if (A050091_cnt == A050091_cap) {
        A050091_cap = A050091_cap ? A050091_cap * 2 : 1024;
        A050091_T = (uint32_t **)realloc(A050091_T, A050091_cap * sizeof(uint32_t *));
        A050091_L = (size_t *)realloc(A050091_L, A050091_cap * sizeof(size_t));
        A050091_H = (uint64_t *)realloc(A050091_H, A050091_cap * sizeof(uint64_t));
    }
    if ((A050091_cnt + 1) * 2 > A050091_tabcap) {
        size_t i;
        A050091_tabcap = A050091_tabcap ? A050091_tabcap * 2 : 4096;
        free(A050091_tab);
        A050091_tab = (size_t *)calloc(A050091_tabcap, sizeof(size_t));
        for (i = 0; i < A050091_cnt; i++) A050091_tab_insert(i);
    }
    A050091_T[A050091_cnt] = p;
    A050091_L[A050091_cnt] = len;
    A050091_H[A050091_cnt] = h;
    A050091_tab_insert(A050091_cnt);
    A050091_cnt++;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A050091_cnt == 0) {
        uint32_t *one = (uint32_t *)malloc(sizeof(uint32_t));
        one[0] = 1;
        A050091_add(one, 1, A050091_hash(one, 1));
    }
    while (A050091_dcnt < (size_t)n) {
        size_t k = A050091_cnt;
        uint32_t *prev = A050091_T[k - 1];
        size_t plen = A050091_L[k - 1];
        uint32_t *cand = (uint32_t *)malloc(plen * sizeof(uint32_t));
        uint64_t rem = 0;
        size_t i, clen = plen;
        bool used = false;
        for (i = plen; i-- > 0;) {
            uint64_t cur = (rem << 32) | prev[i];
            cand[i] = (uint32_t)(cur / 3);
            rem = cur % 3;
        }
        while (clen > 0 && cand[clen - 1] == 0) clen--;
        if (clen > 0) {
            uint64_t h = A050091_hash(cand, clen);
            if (!A050091_find(cand, clen, h)) {
                A050091_add(cand, clen, h);
                used = true;
                if (A050091_dcnt == A050091_dcap) {
                    A050091_dcap = A050091_dcap ? A050091_dcap * 2 : 1024;
                    A050091_D = (size_t *)realloc(A050091_D, A050091_dcap * sizeof(size_t));
                }
                A050091_D[A050091_dcnt++] = k;
            }
        }
        if (!used) {
            uint32_t *nx = (uint32_t *)malloc((plen + 1) * sizeof(uint32_t));
            uint64_t carry = 0;
            size_t nlen = plen + 1;
            free(cand);
            for (i = 0; i < plen; i++) {
                uint64_t cur = (uint64_t)prev[i] * 6 + carry;
                nx[i] = (uint32_t)cur;
                carry = cur >> 32;
            }
            nx[plen] = (uint32_t)carry;
            while (nlen > 0 && nx[nlen - 1] == 0) nlen--;
            A050091_add(nx, nlen, A050091_hash(nx, nlen));
        }
    }
    return (seq_t)A050091_D[(size_t)n - 1];
}
