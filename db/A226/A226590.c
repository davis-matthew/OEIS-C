/*
 * A226590  Total number of 0's in binary expansion of all divisors of n.
 * https://oeis.org/A226590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{d|n} ( ⌊log₂ d⌋ + 1 – popcount(d) ) (total number of
 *            0's in binary expansion of all divisors of n)
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 97 terms through n = 493 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Factor n into prime factors (primes[i]^exps[i]). */
static int local_factorize(uint64_t n, uint64_t *primes, int *exps)
{
    int cnt = 0;
    uint64_t tmp = n;

    if (tmp == 0) return 0;          /* guard against zero, though n ≥ 1 */

    /* Handle factor 2 separately to keep loop odd. */
    if ((tmp & 1ULL) == 0) {
        int exp = 0;
        while ((tmp & 1ULL) == 0) {
            ++exp;
            tmp >>= 1;
        }
        primes[cnt] = 2ULL;
        exps[cnt]   = exp;
        ++cnt;
    }

    /* Odd factors up to sqrt(tmp). */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            primes[cnt] = p;
            exps[cnt]   = exp;
            ++cnt;
        }
    }

    /* Remaining prime factor > 1. */
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    return cnt;
}

/* Count 0's in binary representation of d (uint64_t). */
static unsigned int zeros_in_divisor(uint64_t d)
{
    unsigned int bitlen = 0;
    uint64_t tmp = d;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }
    unsigned int ones = __builtin_popcountll(d);
    return bitlen - ones;
}

/* Recursively generate all divisors and accumulate their zero counts. */
static void gen_divisors(uint64_t current, size_t idx, size_t prime_count,
                         const uint64_t *primes, const int *exps,
                         seq_t *sum)
{
    if (idx == prime_count) {
        *sum += (seq_t)zeros_in_divisor(current);
        return;
    }

    uint64_t val = current;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors(val, idx + 1, prime_count, primes, exps, sum);
        if (e < exps[idx]) val *= primes[idx];
    }
}

/* External linkage required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1; return 0 for safety */

    uint64_t nn = (uint64_t)n;

    uint64_t primes[64];
    int exps[64];
    int prime_count = local_factorize(nn, primes, exps);

    seq_t sum = 0;
    gen_divisors(1ULL, 0, (size_t)prime_count, primes, exps, &sum);
    return sum;
}
