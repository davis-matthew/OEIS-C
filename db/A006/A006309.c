/*
 * A006309  Coefficients of period polynomials.
 * https://oeis.org/A006309
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = [x^(p-3)] f(p,x), p = A000040(n), with f(p,x) the period
 *            polynomial of the degree-p subfield of Q(zeta_{p^2}); by
 *            Newton's identities a(n) = -(1/3)*Sum eta^3 = p*((p-2) - p*N)/3,
 *            where N = #{(h1,h2) : h1, h2 (p-1)-th roots of unity mod p^2,
 *            1+h1+h2 == 0 (mod p^2)}; N is counted using the T...
 * Range:     i8 n <= 7, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *pr_list = NULL;
static uint64_t pr_count = 0;

static void sieve_primes(uint64_t limit)
{
    char *comp = (char *)calloc(limit + 1, 1);
    uint64_t i, j, cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            cnt++;
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        }
    }
    free(pr_list);
    pr_list = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    pr_count = 0;
    for (i = 2; i <= limit; i++)
        if (!comp[i]) pr_list[pr_count++] = i;
    free(comp);
}

static uint64_t nth_prime(idx_t n)
{
    uint64_t limit = 1024;
    while (pr_count < n) {
        while (limit / 12 < n) limit *= 2;
        sieve_primes(limit);
        limit *= 2;
    }
    return pr_list[n - 1];
}

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod(r, b, m);
        b = mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    uint64_t m = p * p;
    uint64_t x, N = 0;
    seq_t P;
    for (x = 1; x + 2 <= p; x++) {
        uint64_t y = p - 1 - x;
        uint64_t tx = powmod(x, p, m);
        uint64_t ty = powmod(y, p, m);
        if ((tx + ty + 1) % m == 0) N++;
    }
    P = (seq_t)p;
    return P * ((P - 2) - P * (seq_t)N) / 3;
}
