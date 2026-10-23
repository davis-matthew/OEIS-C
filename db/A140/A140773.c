/*
 * A140773  Consider the products of all pairs of (not necessarily distinct)
 *          positive divisors of n. a(n) is the number of these products that
 *          divide n. a(n) also is the number of the products that are
 *          divisible by n.
 * https://oeis.org/A140773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m|n} ceil(d(m)/2) where d(m) is the number of
 *            divisors of m
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper: enumerate all divisor exponent combinations.
 * exps      – array of exponents of prime factors of n
 * idx       – current prime index being processed
 * cnt       – total number of distinct prime factors
 * d_val     – product of (exponent+1) for already processed primes
 * total     – accumulator for the sum of ceil(d_val/2) over all divisors
 */
static void dfs(const uint8_t *exps, size_t idx, size_t cnt,
                seq_t d_val, seq_t *total)
{
    if (idx == cnt) {
        /* ceil(d_val/2) = (d_val + 1) / 2 with integer truncation */
        seq_t add = (d_val + 1) >> 1;
        *total += add;
        return;
    }

    uint8_t e = exps[idx];
    for (uint8_t f = 0; f <= e; ++f) {
        seq_t next = d_val * (seq_t)(f + 1);
        dfs(exps, idx + 1, cnt, next, total);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */
    uint64_t temp = (uint64_t)n;

    uint64_t primes[64];
    uint8_t exps[64];
    size_t cnt = 0;

    /* factor 2 */
    uint8_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e) {
        primes[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }

    /* remaining prime >1 */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t total = 0;
    dfs(exps, 0, cnt, (seq_t)1, &total);
    return total;
}
