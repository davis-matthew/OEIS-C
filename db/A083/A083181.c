/*
 * A083181  a(1)=1, a(n) = smallest positive integer not included earlier such
 *          that the partial sums of a(A000040) are composite and the partial
 *          sums of a(A002808) are primes.
 * https://oeis.org/A083181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = smallest unused positive integer m such
 *            that, with n prime, the sum of a(p) over primes p<=n is
 *            composite, and, with n composite, the sum of a(c) over
 *            composites c<=n is prime. Greedy search
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A083181_isprime_idx(idx_t x)
{
    idx_t i;
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (i = 3; i <= x / i; i += 2)
        if (x % i == 0) return false;
    return true;
}

static bool A083181_isprime_seq(seq_t x)
{
    seq_t i;
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    if (x % 3 == 0) return false;
    for (i = 5; i <= x / i; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

static seq_t *A083181_terms = NULL;
static size_t A083181_cap = 0;
static size_t A083181_cnt = 0;
static unsigned char *A083181_used = NULL;
static size_t A083181_ucap = 0;
static seq_t A083181_sp = 0, A083181_sc = 0, A083181_minfree = 1;

static void A083181_grow_used(size_t need)
{
    size_t nc = A083181_ucap ? A083181_ucap : 1024;
    size_t i;
    unsigned char *nu;
    while (nc <= need) nc *= 2;
    nu = (unsigned char *)calloc(nc, 1);
    for (i = 0; i < A083181_ucap; i++) nu[i] = A083181_used[i];
    free(A083181_used);
    A083181_used = nu;
    A083181_ucap = nc;
}

seq_t a(idx_t n)
{
    idx_t k;
    if (n < 1) return (seq_t)0;
    if ((size_t)n > A083181_cap) {
        size_t nc = A083181_cap ? A083181_cap * 2 : 128;
        size_t i;
        seq_t *nt;
        if (nc < (size_t)n) nc = (size_t)n;
        nt = mem_new(seq_t, nc);
        for (i = 0; i < A083181_cnt; i++) nt[i] = A083181_terms[i];
        if (A083181_terms) mem_free(A083181_terms);
        A083181_terms = nt;
        A083181_cap = nc;
    }
    for (k = (idx_t)A083181_cnt + 1; k <= n; k++) {
        seq_t m;
        bool kp = (k > 1) && A083181_isprime_idx(k);
        bool kc = (k > 1) && !kp;
        if (k == 1) {
            m = 1;
        } else {
            m = A083181_minfree;
            for (;;) {
                bool ok;
                if ((size_t)m >= A083181_ucap) A083181_grow_used((size_t)m);
                if (!A083181_used[(size_t)m]) {
                    if (kp) {
                        seq_t s = A083181_sp + m;
                        ok = !A083181_isprime_seq(s) && s > 1;
                    } else {
                        ok = A083181_isprime_seq(A083181_sc + m);
                    }
                    if (ok) break;
                }
                m++;
            }
        }
        if ((size_t)m >= A083181_ucap) A083181_grow_used((size_t)m);
        A083181_used[(size_t)m] = 1;
        if (kp) A083181_sp += m;
        if (kc) A083181_sc += m;
        while ((size_t)A083181_minfree < A083181_ucap && A083181_used[(size_t)A083181_minfree])
            A083181_minfree++;
        A083181_terms[A083181_cnt++] = m;
    }
    return A083181_terms[n - 1];
}
