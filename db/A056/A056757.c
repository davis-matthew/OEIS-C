/*
 * A056757  Cube of number of divisors is larger than the number.
 * https://oeis.org/A056757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 4856, u32 n <= 51126, u64 n <= 51261,
 *            u128 n <= 51261, big any n
 * Validated: full, pass, 51261 terms through n = 51261 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A056757: numbers k with tau(k)^3 > k.
 *
 * Every term is found by a search over prime factorizations k = prod p^e.
 * Multiplying k by p^e multiplies r = tau(k)^3 / k by (e+1)^3 / p^e, which is
 * at most f(p) = max_e (e+1)^3 / p^e: 8 for p = 2, 3 for 3, 8/5 for 5, 8/7
 * for 7, and below 1 for every larger prime. So from a partial product the
 * ratio can still grow by at most the product of f(p) over the primes not yet
 * used that exceed 1; a branch where even that cannot lift r above 1 holds no
 * term. That bound makes the search finite (the largest term is 27935107200)
 * and complete. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static const unsigned PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191,
    193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
    307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
};
enum { NP = sizeof PRIMES / sizeof PRIMES[0] };

/* the largest growth of tau^3/k still possible using primes from index i on */
static double growth(size_t i)
{
    static const double f[4] = {8.0, 3.0, 8.0 / 5.0, 8.0 / 7.0};
    double g = 1.0;
    for (size_t j = i; j < 4; ++j) g *= f[j];
    return g;
}

static void push(uint64_t k)
{
    if (term_len == term_cap) {
        size_t cap = term_cap ? 2 * term_cap : 65536;
        uint64_t *t = (uint64_t *)realloc(terms, cap * sizeof *t);
        if (!t) abort();
        terms = t;
        term_cap = cap;
    }
    terms[term_len++] = k;
}

/* every term k * m where m uses only primes from index i on; tau = tau(k) */
static void search(size_t i, uint64_t k, uint64_t tau)
{
    if (tau * tau * tau > k) push(k);
    for (size_t j = i; j < NP; ++j) {
        const uint64_t p = PRIMES[j];
        /* this prime and all larger ones scale the ratio by at most this */
        double best = (double)tau * tau * tau / (double)k * growth(j) * (j < 4 ? 1.0 : 8.0 / p);
        if (best <= 1.0) break;
        uint64_t kk = k;
        for (uint64_t e = 1; kk <= UINT64_C(1) << 40; ++e) {
            kk *= p;
            uint64_t t = tau * (e + 1);
            if ((double)t * t * t / (double)kk * growth(j + 1) > 1.0) {
                search(j + 1, kk, t);
            } else if ((double)(e + 2) * (e + 2) * (e + 2) < (double)p * (e + 1) * (e + 1) * (e + 1)) {
                break;        /* (e+1)^3 / p^e is past its maximum: higher powers do worse */
            }
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
        search(0, 1, 1);
        qsort(terms, term_len, sizeof *terms, cmp_u64);
    }
    if (n < 1 || (size_t)n > term_len) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
