/*
 * A118585  Sum of squares of digits of prime factors of n, with multiplicity.
 * https://oeis.org/A118585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{p^e‖n} e * (sum of squares of digits of p)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper to compute sum of squares of decimal digits of a uint64_t */
static inline uint64_t digit_square_sum_uint64(uint64_t x)
{
    uint64_t s = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        s += d * d;
        x /= 10;
    }
    return s;
}

/* External type definitions and helpers are provided by the build system. */
extern seq_t a(idx_t n) /* main sequence function */
{
    /* By convention a(1) = 0. */
    if (n <= 1)
        return (seq_t)0;

    uint64_t n_val = (uint64_t)n;
    uint64_t temp = n_val;
    seq_t result = 0;

    /* Factorization by trial division – sufficient for n ≤ 10000. */
    /* Handle factor 2 separately for speed. */
    uint64_t count = 0;
    while ((temp & 1) == 0) {   /* temp % 2 == 0 */
        temp >>= 1;
        count++;
    }
    if (count > 0) {
        result += (seq_t)count * (seq_t)digit_square_sum_uint64(2);
    }

    /* Odd factors starting from 3 up to sqrt(temp). */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            count = 0;
            while (temp % p == 0) {
                temp /= p;
                count++;
            }
            result += (seq_t)count * (seq_t)digit_square_sum_uint64(p);
        }
    }

    /* Remaining prime > sqrt(original n). */
    if (temp > 1) {
        result += (seq_t)1 * (seq_t)digit_square_sum_uint64(temp);
    }

    return result;
}
