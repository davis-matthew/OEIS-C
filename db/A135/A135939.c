/*
 * A135939  Highest exponent occurring in the prime factorization of
 *          Fibonacci(n).
 * https://oeis.org/A135939
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Highest exponent occurring in the prime factorization of
 *            Fibonacci(n) (OEIS A135939)
 * Range:     u8 n <= 1422, u16 n <= 1422, u32 n <= 1422, u64 n <= 1422,
 *            u128 n <= 1422, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* cache of ranks of apparition for primes up to 10000 */
static idx_t rank_cache[10001] = {0};

/* compute (and cache) the rank of apparition of a prime p:
   smallest d > 0 such that p divides Fibonacci(d) */
static idx_t
rank_of_prime(uint32_t p)
{
    if (rank_cache[p])
        return rank_cache[p];
    if (p == 2) {
        rank_cache[p] = 3;   /* rank of 2 is 3 */
        return 3;
    }

    /* compute Fibonacci modulo p until a term is 0 */
    idx_t a = 0, b = 1, k = 0, c;
    do {
        ++k;
        c = a + b;
        if (c >= p)
            c -= p;
        a = b;
        b = c;
    } while (b != 0);
    rank_cache[p] = k + 1;   /* index of the zero term */
    return rank_cache[p];
}

/* factor m into primes and for each prime that has rank d update maxExp */
static void
factor_and_process(idx_t m, idx_t d, seq_t *maxExp)
{
    idx_t temp = m;
    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp_in_m = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp_in_m;
            }
            idx_t rank = rank_of_prime((uint32_t)p);
            if (rank == d) {
                seq_t cand = (seq_t)(exp_in_m + 1);
                if (cand > *maxExp)
                    *maxExp = cand;
            }
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        idx_t p = temp;
        idx_t exp_in_m = 1;
        idx_t rank = rank_of_prime((uint32_t)p);
        if (rank == d) {
            seq_t cand = (seq_t)(exp_in_m + 1);
            if (cand > *maxExp)
                *maxExp = cand;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* unspecified for n < 3 */

    seq_t maxExp = 1;   /* at least one prime factor in every Fibonacci(n) for n >= 3 */

    /* special treatment for prime 2 */
    if (n % 3 == 0) {
        if (n % 6 == 0) {
            idx_t tmp = n;
            seq_t v2 = 0;
            while ((tmp & 1) == 0) {
                tmp >>= 1;
                ++v2;
            }
            seq_t exp = v2 + 2;
            if (exp > maxExp)
                maxExp = exp;
        } else {
            if (1 > maxExp)
                maxExp = 1;
        }
    }

    /* special treatment for prime 5 */
    {
        idx_t tmp = n;
        seq_t v5 = 0;
        while (tmp % 5 == 0) {
            tmp /= 5;
            ++v5;
        }
        if (v5 > maxExp)
            maxExp = v5;
    }

    /* process all divisors d of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t m1 = n / d1;
            factor_and_process(m1, d1, &maxExp);

            if (i != n / i) {
                idx_t d2 = n / i;
                idx_t m2 = n / d2;
                factor_and_process(m2, d2, &maxExp);
            }
        }
    }

    return maxExp;
}
