/*
 * A175649  a(n) = sopf(n) + sopf(n+1).
 * https://oeis.org/A175649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopf(n) + sopf(n+1) where sopf(m) = sum of distinct prime
 *            factors of m
 * Range:     u8 n <= 177, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 975 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of distinct prime factors of a 64‑bit unsigned integer.
   The result is returned in type seq_t to match the build. */
static seq_t distinct_prime_factor_sum(uint64_t x)
{
    seq_t sum = 0;

    if (x % 2ULL == 0ULL) {
        sum += (seq_t)2;
        while (x % 2ULL == 0ULL)
            x /= 2ULL;
    }

    /* Iteration up to sqrt(x) */
    for (uint64_t p = 3ULL; p * p <= x; p += 2ULL) {
        if (x % p == 0ULL) {
            sum += (seq_t)p;
            while (x % p == 0ULL)
                x /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (x > 1ULL)
        sum += (seq_t)x;

    return sum;
}

seq_t a(idx_t n)
{
    /* n and n+1 fit into uint64_t for all intended test ranges */
    seq_t sum1 = distinct_prime_factor_sum((uint64_t)n);
    seq_t sum2 = distinct_prime_factor_sum((uint64_t)(n + 1));
    return sum1 + sum2;
}
