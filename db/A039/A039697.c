/*
 * A039697  a(n) = Sum(p_j) * Sum(k_j) where n = Product(p_j^k_j).
 * https://oeis.org/A039697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum(p_j) * Sum(k_j) where n = product(p_j^k_j)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* a(n) is defined for n >= 1.  The function returns 0 for n = 1. */
seq_t a(idx_t n)
{
    /* n is at least 1, but we return 0 for n==1 as required. */
    if (n <= 1)
        return 0;

    /* Work on a copy of n for factorisation. */
    uint64_t m = (uint64_t)n;

    seq_t sum_primes = 0;
    seq_t sum_exps   = 0;

    /* Handle factor 2 separately for speed. */
    int exp = 0;
    while ((m & 1ULL) == 0) {        /* while m % 2 == 0 */
        m >>= 1;                    /* m /= 2 */
        exp++;
    }
    if (exp > 0) {
        sum_primes += (seq_t)2;
        sum_exps   += (seq_t)exp;
    }

    /* Trial divide by odd numbers up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        if (exp > 0) {
            sum_primes += (seq_t)p;
            sum_exps   += (seq_t)exp;
        }
    }

    /* If something remains, it is a prime factor > sqrt(original n). */
    if (m > 1) {
        sum_primes += (seq_t)m;
        sum_exps   += (seq_t)1;
    }

    return sum_primes * sum_exps;
}
