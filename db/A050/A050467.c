/*
 * A050467  a(n) = Sum_{d|n, n/d=3 mod 4} d^4.
 * https://oeis.org/A050467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, (n/d) mod 4 == 3} d^4
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 767, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8366 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only standard headers and a simple trial‑division factorization. */
#include <stdint.h>
#include <stddef.h>

/* Recursively enumerate all divisors of n using the prime factorization
   stored in arrays 'pr' and 'exp'.  For each divisor d, if the
   complementary divisor e = n/d satisfies e % 4 == 3, add d^4 to *sum. */
static void
enumerate_divs_rec(uint64_t n, uint64_t cur, size_t idx,
                  const uint64_t *pr, const uint64_t *exp, size_t cnt,
                  seq_t *sum)
{
    if (idx == cnt) {
        uint64_t e = n / cur;
        if ((e & 3ULL) == 3ULL) {          /* e % 4 == 3 */
            seq_t d   = (seq_t)cur;
            seq_t d2  = d * d;             /* d^2 */
            seq_t d4  = d2 * d2;           /* d^4 */
            *sum += d4;
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t mult = 1ULL;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        enumerate_divs_rec(n, cur * mult, idx + 1,
                           pr, exp, cnt, sum);
        mult *= p;
    }
}

/* Factor n into primes and exponents in arrays 'pr' and 'exp'.
   The number of distinct prime factors is written to *cnt. */
static void
factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;

    /* 2 as a special case for efficiency */
    if (m % 2ULL == 0ULL) {
        uint64_t e = 0;
        while (m % 2ULL == 0ULL) {
            m /= 2ULL;
            ++e;
        }
        pr[c]   = 2ULL;
        exp[c++] = e;
    }

    /* Test odd divisors up to sqrt(m) */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            uint64_t e = 0;
            while (m % p == 0ULL) {
                m /= p;
                ++e;
            }
            pr[c]   = p;
            exp[c++] = e;
        }
    }

    if (m > 1ULL) {         /* remaining prime factor > sqrt(n) */
        pr[c]   = m;
        exp[c++] = 1;
    }
    *cnt = c;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset protection */

    uint64_t N = (uint64_t)n;

    /* arrays large enough for any factorization of a 64‑bit integer */
    uint64_t prime[64];
    uint64_t expon[64];
    size_t   cnt = 0;

    factor_uint64(N, prime, expon, &cnt);

    seq_t sum = 0;
    enumerate_divs_rec(N, 1ULL, 0, prime, expon, cnt, &sum);
    return sum;
}
