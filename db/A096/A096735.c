/*
 * A096735  Least k such that in sequence A096982 a(k) = P(n) ( with P(i) =
 *          i-th prime ) or 0 if P(n) is not present in the sequence A096982.
 * https://oeis.org/A096735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A096982(k) = prime(n), i.e. Pisano period
 *            pi(k) = prime(n)-1 (k>=2); 0 if none (computed by sieve search
 *            over k <= 2^21 using pi(k)=lcm of pi(q^e))
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/nth_prime.h"

#define PL_LIMIT 2097152u

static uint32_t *pl_per = 0;

static void pl_fibpair(uint64_t n, uint64_t q, uint64_t *f, uint64_t *g)
{
    uint64_t a = 0, b = 1 % q; /* F(0), F(1) */
    int i;
    for (i = 63; i >= 0; --i) {
        uint64_t c = (a * ((2 * b + q - a) % q)) % q;
        uint64_t d = (a * a + b * b) % q;
        if ((n >> i) & 1) { a = d; b = (c + d) % q; }
        else { a = c; b = d; }
    }
    *f = a; *g = b;
}

static int pl_isper(uint64_t d, uint64_t q)
{
    uint64_t f, g;
    pl_fibpair(d, q, &f, &g);
    return f == 0 && g == 1 % q;
}

static uint64_t pl_prime_period(uint64_t q)
{
    uint64_t N, t, x, r;
    if (q == 2) return 3;
    if (q == 5) return 20;
    N = (q % 5 == 1 || q % 5 == 4) ? q - 1 : 2 * (q + 1);
    t = N;
    x = N;
    for (r = 2; r * r <= x; ++r) {
        if (x % r == 0) {
            while (x % r == 0) x /= r;
            while (t % r == 0 && pl_isper(t / r, q)) t /= r;
        }
    }
    if (x > 1) {
        if (t % x == 0 && pl_isper(t / x, q)) t /= x;
    }
    return t;
}

static uint64_t pl_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void pl_init(void)
{
    uint32_t *spf = (uint32_t *)calloc(PL_LIMIT + 1, sizeof(uint32_t));
    uint32_t k, j;
    pl_per = (uint32_t *)calloc(PL_LIMIT + 1, sizeof(uint32_t));
    for (k = 2; k <= PL_LIMIT; ++k) {
        if (spf[k] == 0) {
            for (j = k; j <= PL_LIMIT; j += k)
                if (spf[j] == 0) spf[j] = k;
        }
    }
    pl_per[1] = 1;
    for (k = 2; k <= PL_LIMIT; ++k) {
        uint32_t q = spf[k];
        uint32_t pe = 1, r = k;
        while (r % q == 0) { r /= q; pe *= q; }
        if (r == 1) {
            if (pe == q) pl_per[k] = (uint32_t)pl_prime_period(q);
            else pl_per[k] = pl_per[k / q] * q;
        } else {
            uint64_t a = pl_per[pe], b = pl_per[r];
            pl_per[k] = (uint32_t)(a / pl_gcd(a, b) * b);
        }
    }
    free(spf);
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    uint64_t m = p - 1;
    uint32_t k;
    if (!pl_per) pl_init();
    for (k = 2; k <= PL_LIMIT; ++k) {
        if (pl_per[k] == m) return (seq_t)k;
    }
    return (seq_t)0;
}
