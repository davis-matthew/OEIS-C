/*
 * A057838  Numbers k such that A055079(k) = 2^k.
 * https://oeis.org/A057838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k; k is a term iff A055079(k) = 2^k, i.e. for every
 *            r >= 2 no factorization k+r = f_1*...*f_r (f_1 >= ... >= f_r >=
 *            2) gives Sum_i (f_i - 1)*log_2(prime(i)) < k. If bigomega(k+r) <
 *            r there is no such factorization; otherwise an exact DFS over
 *            the divisors decides
 * Range:     u8 n <= 6, u16 n <= 85, u32 n <= 13925, u64 n <= 13925,
 *            u128 n <= 13925, big any n
 * Validated: standard, pass, 57 terms through n = 8298 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A057838_NP 64
static double A057838_lg[A057838_NP];
static bool A057838_init = false;
static double A057838_K;
static uint64_t A057838_divs[16384];
static int A057838_ndiv;

/* sieve state: composite bitset up to A057838_L, primes up to sqrt(L) */
static uint8_t *A057838_comp = NULL;
static uint64_t A057838_L = 0;
static uint64_t *A057838_pr = NULL;
static size_t A057838_npr = 0;

static void A057838_build_sieve(uint64_t L)
{
    free(A057838_comp);
    free(A057838_pr);
    A057838_comp = (uint8_t *)calloc((size_t)(L / 8 + 1), 1);
    A057838_comp[0] |= 3; /* 0 and 1 */
    for (uint64_t i = 2; i * i <= L; i++) {
        if (A057838_comp[i >> 3] & (1u << (i & 7))) continue;
        for (uint64_t j = i * i; j <= L; j += i)
            A057838_comp[j >> 3] |= (uint8_t)(1u << (j & 7));
    }
    uint64_t s = 2;
    while ((s + 1) * (s + 1) <= L) s++;
    size_t cap = 64;
    A057838_pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    A057838_npr = 0;
    for (uint64_t i = 2; i <= s + 1 && i <= L; i++) {
        if (A057838_comp[i >> 3] & (1u << (i & 7))) continue;
        if (A057838_npr == cap) {
            cap *= 2;
            A057838_pr = (uint64_t *)realloc(A057838_pr, cap * sizeof(uint64_t));
        }
        A057838_pr[A057838_npr++] = i;
    }
    A057838_L = L;
}

static inline bool A057838_isprime(uint64_t x)
{
    if (x < 2) return false;
    return !(A057838_comp[x >> 3] & (1u << (x & 7)));
}

/* log2 of a positive integer, computed bit by bit with no <math.h> */
static double A057838_log2(uint64_t p)
{
    int ip = 0;
    uint64_t t = p;
    while (t > 1) { t >>= 1; ip++; }
    double y = (double)p;
    for (int i = 0; i < ip; i++) y /= 2.0;
    double res = (double)ip, bit = 0.5;
    for (int i = 0; i < 55; i++) {
        y = y * y;
        if (y >= 2.0) { y /= 2.0; res += bit; }
        bit /= 2.0;
    }
    return res;
}

/* true iff bigomega(D) >= r (D <= sieve limit) */
static bool A057838_omega_atleast(uint64_t D, int r)
{
    int cnt = 0;
    uint64_t rem = D;
    for (size_t i = 0; i < A057838_npr; i++) {
        if (cnt >= r) return true;
        if (rem == 1) break;
        if (A057838_isprime(rem)) { cnt++; rem = 1; break; }
        uint64_t p = A057838_pr[i];
        int need = r - cnt;
        uint64_t pw = 1;
        int j;
        for (j = 0; j < need && pw <= rem; j++) pw *= p;
        if (j == need && pw > rem) return false;
        if (pw > rem) return false;
        while (rem % p == 0) { rem /= p; cnt++; }
    }
    if (rem > 1) cnt++;
    return cnt >= r;
}

static void A057838_gen_divs(uint64_t D)
{
    A057838_ndiv = 1;
    A057838_divs[0] = 1;
    uint64_t m = D;
    for (size_t k = 0; k < A057838_npr && m > 1; k++) {
        if (A057838_isprime(m)) break;
        uint64_t p = A057838_pr[k];
        if (p * p > m) break;
        if (m % p) continue;
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        int cur = A057838_ndiv;
        uint64_t pw = 1;
        for (int j = 1; j <= e; j++) {
            pw *= p;
            for (int i = 0; i < cur; i++)
                A057838_divs[A057838_ndiv++] = A057838_divs[i] * pw;
        }
    }
    if (m > 1) {
        int cur = A057838_ndiv;
        for (int i = 0; i < cur; i++)
            A057838_divs[A057838_ndiv++] = A057838_divs[i] * m;
    }
    for (int i = 1; i < A057838_ndiv; i++) {
        uint64_t v = A057838_divs[i];
        int j = i - 1;
        while (j >= 0 && A057838_divs[j] > v) { A057838_divs[j + 1] = A057838_divs[j]; j--; }
        A057838_divs[j + 1] = v;
    }
}

/* true if rem can be split into `left` factors (each in [2, maxf], non-increasing)
   with total cost < K */
static bool A057838_dfs(uint64_t rem, int idx, int left, uint64_t maxf, double cost)
{
    const double lim = A057838_K - 1e-9;
    if (cost >= lim) return false;
    if (left == 0) return rem == 1;
    if (left == 1) {
        if (rem < 2 || rem > maxf) return false;
        return cost + (double)(rem - 1) * A057838_lg[idx] < lim;
    }
    for (int j = A057838_ndiv - 1; j >= 0; j--) {
        uint64_t f = A057838_divs[j];
        if (f > maxf || f > rem) continue;
        if (f < 2) break;
        if (rem % f) continue;
        uint64_t pw = 1;
        int i;
        for (i = 0; i < left && pw < rem; i++) pw *= f;
        if (pw < rem) break;
        double c = cost + (double)(f - 1) * A057838_lg[idx];
        if (c >= lim) continue;
        if (A057838_dfs(rem / f, idx + 1, left - 1, f, c)) return true;
    }
    return false;
}

static bool A057838_is_term(uint64_t k)
{
    if (k < 2) return false;
    /* r = 2: if k+2 is composite (k > 2), a 2-prime number always beats 2^k */
    if (k > 2 && !A057838_isprime(k + 2)) return false;
    A057838_K = (double)k;
    for (int r = 3; r < A057838_NP; r++) {
        uint64_t D = k + (uint64_t)r;
        if (((uint64_t)1 << r) > D) break;
        if (!A057838_omega_atleast(D, r)) continue;
        A057838_gen_divs(D);
        if (A057838_dfs(D, 0, r, D, 0.0)) return false;
    }
    return true;
}

static uint64_t *A057838_terms = NULL;
static size_t A057838_cnt = 0, A057838_cap = 0;
static uint64_t A057838_next = 2;

seq_t a(idx_t n)
{
    if (!A057838_init) {
        int c = 0;
        for (uint64_t p = 2; c < A057838_NP; p++) {
            bool pr = true;
            for (uint64_t d = 2; d * d <= p; d++)
                if (p % d == 0) { pr = false; break; }
            if (pr) A057838_lg[c++] = A057838_log2(p);
        }
        A057838_build_sieve((uint64_t)1 << 16);
        A057838_init = true;
    }
    while (A057838_cnt < (size_t)n) {
        uint64_t k = A057838_next;
        if (k + 128 > A057838_L) A057838_build_sieve(A057838_L * 2);
        if (!(k > 2 && (k & 1) == 0) && A057838_is_term(k)) {
            if (A057838_cnt == A057838_cap) {
                A057838_cap = A057838_cap ? 2 * A057838_cap : 1024;
                A057838_terms = (uint64_t *)realloc(A057838_terms, A057838_cap * sizeof(uint64_t));
            }
            A057838_terms[A057838_cnt++] = k;
        }
        A057838_next++;
    }
    return (seq_t)A057838_terms[n - 1];
}
