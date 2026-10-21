/*
 * A075491  Sum of digits of n minus number of divisors of n.
 * https://oeis.org/A075491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = digit_sum(n) – divisor_count(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* simple digit sum in base 10 */
static seq_t digit_sum_idx(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x != 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* simple divisor (tau) count */
static uint64_t divisor_count_u64(idx_t n)
{
    uint64_t count = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }
    if (m > 1)
        count *= 2;
    return count;
}

extern seq_t a(idx_t n)
{
    seq_t sum = digit_sum_idx(n);
    uint64_t tau = divisor_count_u64(n);
    return sum - (seq_t)tau;
}
