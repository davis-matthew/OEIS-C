/*
 * A051225  Numbers m such that the Bernoulli number B_{2*m} has denominator
 *          30.
 * https://oeis.org/A051225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = the nth even integer m such that the denominator of
 *            B_{2*m} is 30. This is equivalent to: 2*m has no divisor of the
 *            form p-1 with p an odd prime > 5. We generate the terms by
 *            testing each even integer m in increasing order. The test
 *            factors 2*m and enumerates its divisors; if any divisor v...
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _check_bad_divisors, _is_prime_basic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_check_bad_divisors.h"
#include "../utils/_is_prime_basic.h"

/*** small prime sieve for trial division ***/
#define PRIME_LIMIT 100000            /* sufficient for 2*m up to ~1e10 */
static bool _sieve_done = false;
static uint32_t _prime_list[PRIME_LIMIT/10];   /* about 9592 primes <=100000 */
static size_t _prime_cnt = 0;

static void _build_primes(void)
{
    if (_sieve_done) return;
    bool is_comp[PRIME_LIMIT + 1] = {false};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            _prime_list[_prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
        }
    }
    _sieve_done = true;
}

/* ---------------------------------------------------------- */
/*    public sequence function                               */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    _build_primes();                /* ensure prime table ready */

    uint64_t count = 0;
    uint64_t m     = 0;             /* potential term */

    while (count < n) {
        ++m;
        if ((m & 1u) == 0) { /* only even m can give denominator 30 */
            uint64_t d = 2ULL * m;
            /* factor d */
            uint64_t fac[16];
            int      exp[16];
            size_t   fc = 0;
            uint64_t temp = d;
            for (size_t i = 0; i < _prime_cnt; ++i) {
                uint32_t p = _prime_list[i];
                if ((uint64_t)p * p > temp) break;
                if (temp % p == 0) {
                    fac[fc] = p;
                    int e = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++e;
                    }
                    exp[fc] = e;
                    ++fc;
                }
            }
            if (temp > 1) {
                fac[fc] = temp;
                exp[fc] = 1;
                ++fc;
            }

            int bad = 0;
            _check_bad_divisors(0, 1ULL, fac, exp, fc, &bad);

            if (!bad) {
                ++count;
                /* when we have found the nth term return it */
                if (count == n)
                    return (seq_t)m;
            }
        }
    }
    /* should never reach here */
    return (seq_t)0;
}
