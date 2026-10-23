/*
 * A178601  a(n) = s(s(n)), where s(n) = sigma(n)-n = A001065(n).
 * https://oeis.org/A178601
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(s(n)), where s(n) = σ(n) – n (sum of proper divisors)
 * Range:     u8 n <= 89, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the sum of divisors σ(n) for n ≤ UINT64_MAX.
   This helper uses trial division; it is fast enough for n ≤ 10⁵. */
static seq_t sigma_u64(uint64_t n)
{
    if (n == 0) return 0;    /* undefined, but not used in this context */
    seq_t result = 1;

    uint64_t m = n;
    /* factor 2 */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            e++;
        }
        seq_t sum_p = 1;
        seq_t power = 1;
        for (int i = 0; i < e; ++i) {
            power *= (seq_t)2;
            sum_p += power;
        }
        result *= sum_p;
    }
    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t sum_p = 1;
            seq_t power = 1;
            for (int i = 0; i < e; ++i) {
                power *= (seq_t)p;
                sum_p += power;
            }
            result *= sum_p;
        }
    }
    /* remaining prime factor */
    if (m > 1) {
        seq_t sum_p = (seq_t)1 + (seq_t)m;
        result *= sum_p;
    }
    return result;
}

/* Sum of proper divisors s(n) = σ(n) – n. */
static seq_t s_proper(uint64_t n)
{
    return sigma_u64(n) - n;
}

/* OEIS function a(n) = s(s(n)) */
seq_t a(idx_t n)
{
    /* n ≥ 2 by definition of the offset */
    uint64_t n_u = (uint64_t)n;
    seq_t s1 = s_proper(n_u);
    uint64_t s1_u = (uint64_t)s1;           /* fits into uint64_t for n ≤ 10⁴ */
    seq_t s2 = s_proper(s1_u);
    return s2;
}
