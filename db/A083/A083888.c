/*
 * A083888  Number of divisors of n with largest digit = 1 (base 10).
 * https://oeis.org/A083888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9936 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A083888: a(n) = number of divisors d of n such that the
 * largest decimal digit of d is 1 (i.e. every digit of d is 0 or 1).
 *
 * Computation: factor n, enumerate all of its divisors, and count those
 * whose decimal representation contains only the digits 0 and 1.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether all decimal digits of x are 0 or 1. */
static inline bool only01(uint64_t x)
{
    if (x == 0) return true;          /* not a divisor of any n≠0, but safe */
    while (x > 0) {
        uint8_t d = x % 10;
        if (d > 1) return false;
        x /= 10;
    }
    return true;
}

/* Recursively generate divisors and count those satisfying only01. */
static void count_divs_recursive(int k, uint64_t cur,
                                 const uint64_t *primes, const int *exps,
                                 int total, seq_t *cnt)
{
    if (k == total) {
        if (only01(cur)) (*cnt)++;
        return;
    }
    uint64_t p = primes[k];
    uint64_t val = cur;
    for (int e = 0; e <= exps[k]; ++e) {
        count_divs_recursive(k + 1, val, primes, exps, total, cnt);
        val *= p;               /* next power of p */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but safe */
    uint64_t m = n;
    /* factor n; store primes and exponents */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    if (m % 2 == 0) {
        int e = 0;
        while ((m & 1) == 0) { m >>= 1; ++e; }
        primes[cnt] = 2; exps[cnt] = e; ++cnt;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (m > 1) { primes[cnt] = m; exps[cnt] = 1; ++cnt; }

    seq_t result = 0;
    count_divs_recursive(0, 1, primes, exps, cnt, &result);
    return result;
}
