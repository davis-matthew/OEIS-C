/*
 * A225737  Numbers k such that k < d(k)^(29/10), where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A225737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th integer k such that k^10 < d(k)^29, where d(k)
 *            is the divisor count (k > 0). The sequence begins 2, 3, 4, 5, 6,
 *            7, 8, 9, 10, 12, 14, …
 * Range:     u8 n <= 130, u16 n <= 3826, u32 n <= 21466, u64 n <= 21466,
 *            u128 n <= 21466, big any n
 * Validated: full, pass, 21466 terms through n = 21466 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A225737: numbers k with k < d(k)^(29/10), i.e. k^10 < d(k)^29.
 *
 * As in A056757 (the same with exponent 3): over factorizations k = prod p^e,
 * multiplying k by p^e scales r = d(k)^2.9 / k by (e+1)^2.9 / p^e, whose
 * maximum f(p) over e exceeds 1 only for p = 2, 3, 5, 7. A branch whose ratio
 * cannot reach 1 even with every remaining f(p) > 1 holds no term, so the
 * search is finite (the largest term is 2327925600) and complete. Pruning is
 * done in floating point with a margin; membership is decided exactly in
 * 384-bit arithmetic. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define EXP_NUM 29                /* d^29 against k^10 */
#define EXP_DEN 10

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static const unsigned PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
    193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293
};
enum { NP = sizeof PRIMES / sizeof PRIMES[0], LIMBS = 6 };

/* x^e in LIMBS 64-bit limbs (no overflow for the sizes used here) */
static void power(uint64_t x, unsigned e, uint64_t r[LIMBS])
{
    for (int i = 0; i < LIMBS; ++i) r[i] = 0;
    r[0] = 1;
    while (e--) {
        unsigned __int128 c = 0;
        for (int i = 0; i < LIMBS; ++i) {
            c += (unsigned __int128)r[i] * x;
            r[i] = (uint64_t)c;
            c >>= 64;
        }
    }
}

static int member(uint64_t k, uint64_t d)        /* k^10 < d^29 */
{
    uint64_t l[LIMBS], r[LIMBS];
    power(k, EXP_DEN, l);
    power(d, EXP_NUM, r);
    for (int i = LIMBS - 1; i >= 0; --i)
        if (l[i] != r[i]) return l[i] < r[i];
    return 0;
}

static double g[5];               /* g[i]: product of f(p) > 1 over primes from index i */

static void init_growth(void)
{
    const double t = (double)EXP_NUM / EXP_DEN;
    g[4] = 1.0;
    for (int i = 3; i >= 0; --i) {
        double best = 1.0;
        for (int e = 1; e < 64; ++e) {
            double f = pow(e + 1.0, t) / pow((double)PRIMES[i], e);
            if (f > best) best = f;
        }
        g[i] = g[i + 1] * best;
    }
}

static double ratio(uint64_t k, uint64_t d)
{
    return pow((double)d, (double)EXP_NUM / EXP_DEN) / (double)k;
}

static void push(uint64_t k)
{
    if (term_len == term_cap) {
        size_t cap = term_cap ? 2 * term_cap : 16384;
        uint64_t *t = (uint64_t *)realloc(terms, cap * sizeof *t);
        if (!t) abort();
        terms = t;
        term_cap = cap;
    }
    terms[term_len++] = k;
}

/* every term k * m where m uses only primes from index i on; d = d(k) */
static void search(size_t i, uint64_t k, uint64_t d)
{
    const double slack = 1.0 + 1e-9;
    if (k > 1 && member(k, d)) push(k);
    const double two = pow(2.0, (double)EXP_NUM / EXP_DEN);     /* 2^2.9: e = 1 */
    for (size_t j = i; j < NP; ++j) {
        const uint64_t p = PRIMES[j];
        double best = ratio(k, d) * (j < 4 ? g[j] : two / (double)p);
        if (best * slack <= 1.0) break;           /* larger primes do no better */
        uint64_t kk = k;
        for (unsigned e = 1; kk <= UINT64_C(1) << 40; ++e) {
            kk *= p;
            uint64_t dd = d * (e + 1);
            if (ratio(kk, dd) * g[j + 1 < 4 ? j + 1 : 4] * slack > 1.0)
                search(j + 1, kk, dd);
            else if (pow((e + 2.0) / (e + 1.0), (double)EXP_NUM / EXP_DEN) < (double)p)
                break;            /* (e+1)^2.9 / p^e is falling: higher powers do worse */
        }
    }
}

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    if (!terms) {
        init_growth();
        search(0, 1, 1);
        qsort(terms, term_len, sizeof *terms, cmp_u64);
    }
    if (n < 1 || (size_t)n > term_len) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
