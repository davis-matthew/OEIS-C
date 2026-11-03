/*
 * A072513  Product of all n - d, where d < n and d is a divisor of n.
 * https://oeis.org/A072513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d|n, d<n} (n-d)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 239,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 935 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper: iterate through all divisors and accumulate the product */
static void rec_divs(idx_t n, int idx, seq_t cur,
                     const uint64_t primes[], const int exps[],
                     int cnt, seq_t *res)
{
    if (idx == cnt) {
        if (cur != n) {
            *res *= (seq_t)(n - cur);
        }
        return;
    }
    seq_t val = cur;
    for (int k = 0; k <= exps[idx]; ++k) {
        rec_divs(n, idx + 1, val, primes, exps, cnt, res);
        val *= primes[idx];
    }
}

/* Main function computing the sequence value */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;                  /* a(1) = 1 by definition */
    }

    /* Simple prime factorization */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t tmp = n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    rec_divs(n, 0, (seq_t)1, primes, exps, cnt, &result);
    return result;
}
