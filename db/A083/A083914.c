/*
 * A083914  Number of divisors of n that are congruent to 4 modulo 10.
 * https://oeis.org/A083914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are congruent to 4 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_mod10_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_mod10_rec.h"

static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    int c = 0;
    /* factor 2 */
    if (n % 2ULL == 0) {
        int e = 0;
        while (n % 2ULL == 0) {
            n /= 2ULL;
            e++;
        }
        primes[c] = 2ULL;
        exps[c]   = e;
        c++;
    }
    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            primes[c] = p;
            exps[c]   = e;
            c++;
        }
    }
    if (n > 1ULL) {
        primes[c] = n;
        exps[c]   = 1;
        c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n < 1ULL) return (seq_t)0;
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    factor_uint64(n64, primes, exps, &cnt);
    uint64_t res = 0;
    count_mod10_rec(0, 1ULL, primes, exps, cnt, &res);
    return (seq_t)res;
}
