/*
 * A099635  a(n) = gcd(sum of all prime factors of n, n).
 * https://oeis.org/A099635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sum of distinct prime factors of n, n )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely only on integer operations; no floating point or stdlib calls. */
#include <stdbool.h>

/* Euclidean algorithm for seq_t. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sum of distinct prime factors of an idx_t number. */
static seq_t sum_distinct_prime_factors(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            sum += (seq_t)p;      /* add prime once */
            while (x % p == 0)    /* strip all occurrences */
                x /= p;
        }
    }
    if (x > 1)                 /* remaining prime factor */
        sum += (seq_t)x;
    return sum;
}

/* OEIS A099635 */
seq_t a(idx_t n)
{
    if (n == 0)                /* not used by OEIS (offset = 1) */
        return 0;
    seq_t pf_sum = sum_distinct_prime_factors(n);
    return seq_gcd(pf_sum, (seq_t)n);
}
