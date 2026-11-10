/*
 * A052347  Record values reached in A052343 and A052344.
 * https://oeis.org/A052347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th smallest distinct value among the record values of
 *            A052343(k)=ceil(D/2) and A052344(k)=floor(D/2), where D = prod
 *            (e_i+1) over primes p_i = 1 (mod 4) with exponent e_i in 4k+1;
 *            found by a search over numbers 4k+1 = prod q_i^e_i (q_i =
 *            5,13,17,... , e_i nonincreasing), in increasing order
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

typedef struct { long double lg; unsigned long long D; } A052347_ent;

static A052347_ent *A052347_ents = NULL;
static size_t A052347_ne = 0, A052347_cap = 0;
static long double A052347_L;
static long double A052347_plg[400];
static int A052347_np = 0;

static unsigned long long *A052347_vals = NULL;
static size_t A052347_cnt = 0;

static void A052347_push(long double lg, unsigned long long D)
{
    if (A052347_ne == A052347_cap) {
        A052347_cap = A052347_cap ? A052347_cap * 2 : 1024;
        A052347_ents = (A052347_ent *)realloc(A052347_ents, A052347_cap * sizeof(A052347_ent));
    }
    A052347_ents[A052347_ne].lg = lg;
    A052347_ents[A052347_ne].D = D;
    A052347_ne++;
}

static void A052347_dfs(int idx, int maxe, long double lg, unsigned long long D)
{
    A052347_push(lg, D);
    if (idx >= A052347_np) return;
    for (int e = 1; e <= maxe; e++) {
        long double nl = lg + A052347_plg[idx] * e;
        if (nl > A052347_L) break;
        A052347_dfs(idx + 1, e, nl, D * (unsigned long long)(e + 1));
    }
}

static int A052347_cmp_ent(const void *a, const void *b)
{
    long double x = ((const A052347_ent *)a)->lg, y = ((const A052347_ent *)b)->lg;
    return (x > y) - (x < y);
}

static int A052347_cmp_u(const void *a, const void *b)
{
    unsigned long long x = *(const unsigned long long *)a, y = *(const unsigned long long *)b;
    return (x > y) - (x < y);
}

static void A052347_build(long double L)
{
    if (A052347_np == 0) {
        for (int p = 5; p < 8000 && A052347_np < 400; p++) {
            int pr = 1;
            for (int d = 2; d * d <= p; d++) if (p % d == 0) { pr = 0; break; }
            if (pr && p % 4 == 1) A052347_plg[A052347_np++] = logl((long double)p);
        }
    }
    A052347_L = L;
    A052347_ne = 0;
    A052347_dfs(0, 1000, 0.0L, 1ULL);
    qsort(A052347_ents, A052347_ne, sizeof(A052347_ent), A052347_cmp_ent);

    unsigned long long *tmp = (unsigned long long *)malloc(2 * A052347_ne * sizeof(unsigned long long) + 8);
    size_t c = 0;
    unsigned long long maxC = 0, maxF = 0;
    for (size_t i = 0; i < A052347_ne; i++) {
        unsigned long long D = A052347_ents[i].D;
        unsigned long long ce = (D + 1) / 2, fl = D / 2;
        if (ce > maxC) { maxC = ce; tmp[c++] = ce; }
        if (fl > maxF) { maxF = fl; if (fl > 0) tmp[c++] = fl; }
    }
    qsort(tmp, c, sizeof(unsigned long long), A052347_cmp_u);
    size_t u = 0;
    for (size_t i = 0; i < c; i++)
        if (u == 0 || tmp[i] != tmp[u - 1]) tmp[u++] = tmp[i];
    free(A052347_vals);
    A052347_vals = tmp;
    A052347_cnt = u;
}

seq_t a(idx_t n)
{
    static long double curL = 0;
    if (n < 1) return 0;
    while (A052347_cnt < (size_t)n) {
        curL = (curL == 0) ? 40.0L : curL * 1.5L;
        A052347_build(curL);
    }
    return (seq_t)A052347_vals[n - 1];
}
