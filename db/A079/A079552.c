/*
 * A079552  Record values in A079551.
 * https://oeis.org/A079552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A000005(A006093(k)) = Sum_{k=1..n}
 *            d(prime(k)-1), a(0) = 0
 * Range:     u8 n <= 35, u16 n <= 3675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8304 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A079552_pr = NULL;   /* primes found so far */
static size_t A079552_pr_cnt = 0;
static size_t A079552_pr_cap = 0;
static seq_t *A079552_pre = NULL;     /* A079552_pre[k] = a(k) */
static size_t A079552_pre_cap = 0;
static size_t A079552_pre_cnt = 0;    /* number of valid entries (indices 0..cnt-1) */

static uint64_t A079552_dcount(uint64_t m)
{
    uint64_t res = 1;
    for (size_t i = 0; i < A079552_pr_cnt; ++i) {
        uint64_t p = A079552_pr[i];
        if (p * p > m) break;
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            res *= (e + 1);
        }
    }
    if (m > 1) res *= 2;
    return res;
}

static uint64_t A079552_next_prime(void)
{
    uint64_t c = A079552_pr_cnt ? A079552_pr[A079552_pr_cnt - 1] + 1 : 2;
    for (;; ++c) {
        bool ok = true;
        for (size_t i = 0; i < A079552_pr_cnt; ++i) {
            uint64_t p = A079552_pr[i];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (A079552_pre_cnt == 0) {
        A079552_pre_cap = 64;
        A079552_pre = mem_resize(seq_t, A079552_pre, A079552_pre_cap);
        A079552_pre[0] = (seq_t)0;
        A079552_pre_cnt = 1;
    }
    if (need > A079552_pre_cap) {
        size_t nc = A079552_pre_cap;
        while (nc < need) nc *= 2;
        A079552_pre = mem_resize(seq_t, A079552_pre, nc);
        A079552_pre_cap = nc;
    }
    while (A079552_pre_cnt < need) {
        uint64_t p = A079552_next_prime();
        uint64_t d = A079552_dcount(p - 1);
        if (A079552_pr_cnt >= A079552_pr_cap) {
            size_t nc = A079552_pr_cap ? A079552_pr_cap * 2 : 64;
            A079552_pr = (uint64_t *)realloc(A079552_pr, nc * sizeof(uint64_t));
            A079552_pr_cap = nc;
        }
        A079552_pr[A079552_pr_cnt++] = p;
        A079552_pre[A079552_pre_cnt] = A079552_pre[A079552_pre_cnt - 1] + (seq_t)d;
        ++A079552_pre_cnt;
    }
    return A079552_pre[(size_t)n];
}
