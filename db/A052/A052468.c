/*
 * A052468  Numerators in the Taylor series for arccosh(x) - log(2*x).
 * https://oeis.org/A052468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator((2*n-1)!/(4^n*(n!)^2))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   factor_in_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_in_number.h"

static bool primes_initialized = false;
static idx_t primes[3000];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;
    static bool sieve[20001];
    for (idx_t i = 0; i <= 20000; ++i)
        sieve[i] = false;
    for (idx_t i = 2; i * i <= 20000; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= 20000; j += i)
                sieve[j] = true;
        }
    }
    for (idx_t i = 2; i <= 20000; ++i)
        if (!sieve[i])
            primes[primes_cnt++] = i;
    primes_initialized = true;
}

static idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t total = 0;
    while (n) {
        n /= p;
        total += n;
    }
    return total;
}

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    init_primes();
    idx_t limit = n << 1;          /* 2*n */
    seq_t ans = 1;

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        idx_t p = primes[i];

        idx_t e_b = exp_in_fact(limit, p) - (idx_t)2 * exp_in_fact(n, p);
        idx_t e_d;
        if (p == 2) {
            idx_t e_n2 = factor_in_number(n, 2);
            e_d = (limit - 1) + e_n2;   /* (2*n-1) + exponent of 2 in n */
        } else {
            e_d = factor_in_number(n, p);
        }

        if (e_b > e_d) {
            idx_t exp = e_b - e_d;
            seq_t base = (seq_t)p;
            ans *= pow_seq(base, exp);
        }
    }
    return ans;
}
