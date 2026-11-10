/*
 * A229303  Numbers m such that A031971(2*m) == m (mod 2*m).
 * https://oeis.org/A229303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with A031971(2m) == m (mod 2m), where A031971(N) =
 *            Sum_{k=1..N} k^N; found by incremental search, checking each
 *            prime power q || 2m via S = (N/q)*Sum_{k=1..q} k^N (mod q)
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9980 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t pw_mod_q(uint64_t b, uint64_t e, uint64_t q)
{
    unsigned __int128 r = 1 % q, x = b % q;
    while (e) {
        if (e & 1) r = (r * x) % q;
        x = (x * x) % q;
        e >>= 1;
    }
    return (uint64_t)r;
}

static int m_ok(uint64_t m)
{
    uint64_t N = 2 * m, t = N;
    for (uint64_t p = 2; p * p <= t || t > 1; ) {
        if (p * p > t) p = t;
        if (t % p == 0) {
            uint64_t q = 1;
            int e = 0;
            while (t % p == 0) { t /= p; q *= p; e++; }
            uint64_t T;
            if (e == 1 && p > 2) {
                T = (N % (p - 1) == 0) ? p - 1 : 0;
            } else {
                T = 0;
                for (uint64_t k = 1; k <= q; ++k) T = (T + pw_mod_q(k, N, q)) % q;
            }
            unsigned __int128 s = ((unsigned __int128)((N / q) % q) * T) % q;
            if ((uint64_t)s != m % q) return 0;
        }
        if (t == 1) break;
        p += (p == 2) ? 1 : 2;
    }
    return 1;
}

static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_next = 1;

seq_t a(idx_t n)
{
    while (g_cnt < (size_t)n) {
        if (g_cnt == g_cap) {
            g_cap = g_cap ? g_cap * 2 : 1024;
            g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
        }
        if (m_ok(g_next)) g_terms[g_cnt++] = g_next;
        g_next++;
    }
    return (seq_t)g_terms[n - 1];
}
