/*
 * A235865  G-Carmichael numbers: Composite number such that A235863(n)
 *          divides A201629(n).
 * https://oeis.org/A235865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite k with A235863(k) | A201629(k); computed
 *            by incremental search with memoization
 * Range:     u8 n <= 36, u16 n <= 1038, u32 n <= 1038, u64 n <= 1038,
 *            u128 n <= 1038, big any n
 * Validated: standard, pass, 71 terms through n = 1030 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t g_gcd64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int gc_test(uint64_t k)
{
    uint64_t m;
    uint64_t rem = k;
    uint64_t lam = 1;
    int nfac = 0;
    uint64_t e;
    uint64_t p;
    uint64_t t;

    if (k < 4) return 0;
    if ((k & 1) == 0) m = k;
    else m = ((k & 3) == 1) ? (k - 1) : (k + 1);

    if ((rem & 1) == 0) {
        e = 0;
        while ((rem & 1) == 0) { rem >>= 1; e++; }
        nfac++;
        if (e == 1) t = 2;
        else if (e <= 4) t = 4;
        else t = (uint64_t)1 << (e - 2);
        lam = t;
        if (m % lam) return 0;
    }
    for (p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint64_t pw = 1;
            e = 0;
            while (rem % p == 0) { rem /= p; e++; if (e > 1) pw *= p; }
            nfac++;
            t = ((p % 4 == 1) ? (p - 1) : (p + 1)) * pw;
            lam = (lam / g_gcd64(lam, t)) * t;
            if (m % lam) return 0;
        }
    }
    if (rem > 1) {
        nfac++;
        if (nfac == 1) return 0; /* k is prime */
        t = (rem % 4 == 1) ? (rem - 1) : (rem + 1);
        lam = (lam / g_gcd64(lam, t)) * t;
    }
    if (nfac == 1 && k == rem) return 0;
    return (m % lam) == 0;
}

static uint64_t *g_terms = 0;
static uint64_t g_cnt = 0;
static uint64_t g_cap = 0;
static uint64_t g_next = 4;

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    while (g_cnt < idx) {
        if (gc_test(g_next)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 1024;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
            }
            g_terms[g_cnt++] = g_next;
        }
        g_next++;
    }
    return (seq_t)g_terms[idx - 1];
}
