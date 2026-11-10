/*
 * A050077  a(n)=position of n in A050076.
 * https://oeis.org/A050077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A050076; A050076: a(1)=1,
 *            a(k)=floor(a(k-1)/3) if that is nonzero and unused, else
 *            2*a(k-1). Computed by generating A050076 until n appears
 *            (hash-table inverse)
 * Range:     u8 n <= 10, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned long long *H_keys = NULL;
static unsigned long long *H_pos = NULL;
static size_t H_cap = 0;
static size_t H_cnt = 0;
static seq_t H_last;
static unsigned long long H_len = 0;

static size_t H_slot(unsigned long long k, size_t cap)
{
    unsigned long long h = k * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (cap - 1));
}

static void H_insert_raw(unsigned long long *keys, unsigned long long *pos, size_t cap,
                         unsigned long long k, unsigned long long p)
{
    size_t s = H_slot(k, cap);
    while (keys[s] != 0)
        s = (s + 1) & (cap - 1);
    keys[s] = k;
    pos[s] = p;
}

static void H_insert(unsigned long long k, unsigned long long p)
{
    if ((H_cnt + 1) * 2 > H_cap) {
        size_t ncap = H_cap ? H_cap * 2 : 1024;
        unsigned long long *nk = (unsigned long long *)calloc(ncap, sizeof(unsigned long long));
        unsigned long long *np = (unsigned long long *)calloc(ncap, sizeof(unsigned long long));
        if (!nk || !np)
            exit(EXIT_FAILURE);
        for (size_t i = 0; i < H_cap; ++i)
            if (H_keys[i] != 0)
                H_insert_raw(nk, np, ncap, H_keys[i], H_pos[i]);
        free(H_keys);
        free(H_pos);
        H_keys = nk;
        H_pos = np;
        H_cap = ncap;
    }
    H_insert_raw(H_keys, H_pos, H_cap, k, p);
    ++H_cnt;
}

static bool H_find(unsigned long long k, unsigned long long *p)
{
    if (!H_cap)
        return false;
    size_t s = H_slot(k, H_cap);
    while (H_keys[s] != 0) {
        if (H_keys[s] == k) {
            *p = H_pos[s];
            return true;
        }
        s = (s + 1) & (H_cap - 1);
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (H_len == 0) {
        H_last = (seq_t)1;
        H_len = 1;
        H_insert(1ULL, 1ULL);
    }
    unsigned long long key = (unsigned long long)n;
    unsigned long long p;
    while (!H_find(key, &p)) {
        seq_t cand = H_last / (seq_t)3;
        bool used = false;
        if (cand == (seq_t)0) {
            used = true;
        } else {
            unsigned long long q;
            used = H_find((unsigned long long)cand, &q);
        }
        seq_t next = used ? H_last * (seq_t)2 : cand;
        H_last = next;
        ++H_len;
        H_insert((unsigned long long)next, H_len);
    }
    return (seq_t)p;
}
