/*
 * A229304  Numbers n such that A031971(1806*n) <> n (mod 1806*n).
 * https://oeis.org/A229304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer j with A031971(1806*j) != j (mod
 *            1806*j); computed by a search, testing each prime power q=p^e of
 *            m=1806*j via sum_{k=1..q} k^m (closed form for e=1)
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t a229304_pm(uint64_t b, uint64_t e, uint64_t q)
{
    unsigned __int128 r = 1 % q, x = b % q;
    while (e) {
        if (e & 1) r = (r * x) % q;
        x = (x * x) % q;
        e >>= 1;
    }
    return (uint64_t)r;
}

/* returns 1 if j satisfies A031971(1806*j) == j (mod 1806*j) */
static int a229304_good(uint64_t j)
{
    uint64_t m = 1806ULL * j;
    uint64_t rem = m;
    uint64_t ps[40]; int es[40]; int cnt = 0;
    uint64_t p;
    for (p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            ps[cnt] = p; es[cnt] = e; cnt++;
        }
    }
    if (rem > 1) { ps[cnt] = rem; es[cnt] = 1; cnt++; }
    for (int i = 0; i < cnt; i++) {
        uint64_t pp = ps[i], q = 1, T;
        for (int t = 0; t < es[i]; t++) q *= pp;
        if (es[i] == 1) {
            T = (m % (pp - 1) == 0) ? (pp - 1) : 0;
        } else {
            unsigned __int128 s = 0;
            for (uint64_t k = 1; k <= q; k++) {
                if (k % pp == 0) continue;
                s += a229304_pm(k, m, q);
            }
            T = (uint64_t)(s % q);
        }
        unsigned __int128 lhs = ((unsigned __int128)((m / q) % q) * T) % q;
        if ((uint64_t)lhs != j % q) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 1;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        if (!a229304_good(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next;
        }
        next++;
    }
    return (seq_t)terms[n - 1];
}
