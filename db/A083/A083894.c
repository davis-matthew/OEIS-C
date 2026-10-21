/*
 * A083894  Number of divisors of n with largest digit = 7 (base 10).
 * https://oeis.org/A083894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n with largest decimal digit = 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divisors_rec, largest_digit_is_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_divisors_rec.h"
#include "../utils/largest_digit_is_seven.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N = (uint64_t)n;

    /* Factorise N (trial division).  At most 64 distinct primes. */
    uint64_t primes[64];
    int exps[64];
    int pc = 0;
    uint64_t temp = N;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]  = e;
            ++pc;
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        primes[pc] = temp;
        exps[pc]  = 1;
        ++pc;
    }

    seq_t result = 0;
    count_divisors_rec(1, 0, primes, exps, pc, &result);
    return result;
}
