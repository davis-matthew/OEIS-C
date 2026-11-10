/*
 * A096729  Where records occur in A096287.
 * https://oeis.org/A096729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Let b(m) = A096287(m), the number of steps of m -> m +
 *            (product of nonzero digits of m) before the trajectory of m
 *            reaches the orbit of 1 (A063108). It is computed for every m <=
 *            N from the top down with the rule b(m) = 0 if m is in the orbit
 *            and b(m) = b(m + p(m)) + 1 otherwise. N is dou...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned long long *P096729_orb = NULL;
static size_t P096729_osz = 0, P096729_ocap = 0;

static unsigned long long P096729_dp(unsigned long long x)
{
    unsigned long long p = 1;
    while (x) {
        unsigned long long d = x % 10;
        if (d) p *= d;
        x /= 10;
    }
    return p;
}

static void P096729_ensure(unsigned long long limit)
{
    if (P096729_osz == 0) {
        P096729_ocap = 1024;
        P096729_orb = (unsigned long long *)malloc(P096729_ocap * sizeof(unsigned long long));
        if (!P096729_orb) abort();
        P096729_orb[P096729_osz++] = 1;
    }
    while (P096729_orb[P096729_osz - 1] < limit) {
        unsigned long long c = P096729_orb[P096729_osz - 1];
        if (P096729_osz >= P096729_ocap) {
            P096729_ocap *= 2;
            P096729_orb = (unsigned long long *)realloc(P096729_orb, P096729_ocap * sizeof(unsigned long long));
            if (!P096729_orb) abort();
        }
        P096729_orb[P096729_osz++] = c + P096729_dp(c);
    }
}

static bool P096729_in(unsigned long long x)
{
    size_t lo = 0, hi = P096729_osz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (P096729_orb[mid] < x) lo = mid + 1; else hi = mid;
    }
    return lo < P096729_osz && P096729_orb[lo] == x;
}

static uint32_t *P096729_f = NULL;      /* f[m] = A096287(m) for m <= FN */
static unsigned long long P096729_FN = 0;
static unsigned long long *P096729_rec = NULL;
static size_t P096729_nrec = 0, P096729_rcap = 0;
static long long P096729_best = -1;

static void P096729_extend(unsigned long long N)
{
    unsigned long long m;
    size_t idx;
    P096729_f = (uint32_t *)realloc(P096729_f, ((size_t)N + 1) * sizeof(uint32_t));
    if (!P096729_f) abort();
    P096729_ensure(2 * N + 100);

    /* idx = largest index with orb[idx] <= N */
    {
        size_t lo = 0, hi = P096729_osz;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (P096729_orb[mid] <= N) lo = mid + 1; else hi = mid;
        }
        idx = lo - 1;
    }

    for (m = N; m > P096729_FN; m--) {
        while (P096729_orb[idx] > m) idx--;
        if (P096729_orb[idx] == m) { P096729_f[m] = 0; continue; }
        {
            unsigned long long nx = m + P096729_dp(m);
            if (nx <= N) {
                P096729_f[m] = P096729_f[nx] + 1;
            } else {
                unsigned long long c = nx;
                uint32_t s = 0;
                P096729_ensure(c);
                while (!P096729_in(c)) {
                    c += P096729_dp(c);
                    s++;
                    P096729_ensure(c);
                }
                P096729_f[m] = s + 1;
            }
        }
    }

    for (m = P096729_FN + 1; m <= N; m++) {
        if ((long long)P096729_f[m] > P096729_best) {
            P096729_best = (long long)P096729_f[m];
            if (P096729_nrec >= P096729_rcap) {
                P096729_rcap = P096729_rcap ? P096729_rcap * 2 : 64;
                P096729_rec = (unsigned long long *)realloc(P096729_rec, P096729_rcap * sizeof(unsigned long long));
                if (!P096729_rec) abort();
            }
            P096729_rec[P096729_nrec++] = m;
        }
    }
    P096729_FN = N;
}

seq_t a(idx_t n)
{
    while ((unsigned long long)P096729_nrec < (unsigned long long)n) {
        unsigned long long N = P096729_FN ? P096729_FN * 2 : (1ULL << 20);
        P096729_extend(N);
    }
    return (seq_t)P096729_rec[n - 1];
}
