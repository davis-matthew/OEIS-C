/*
 * A133477  Sum of cubefree divisors of n excluding 1.
 * https://oeis.org/A133477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of cubefree divisors of n excluding 1 (A133477)
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_divs_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sum_divs_recursive.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = n;
    /* We'll never have more than 32 distinct prime factors for n up to 1e12 */
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {                /* Remaining prime factor */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t sum = 0;
    sum_divs_recursive(0, (seq_t)1, primes, exps, cnt, &sum);
    return sum;
}
