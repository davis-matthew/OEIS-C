/*
 * A036453  a(n) = d(d(d(d(d(n))))), the 5th iterate of the number-of-divisors
 *          function d = A000005, with initial value n.
 * https://oeis.org/A036453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(d(d(d(d(n))))) where d = number-of-divisors function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count(seq_t n)
{
    seq_t cnt = 0;
    for (seq_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            cnt += 2;
            if (i * i == n) {
                /* count sqrt divisor only once */
                cnt -= 1;
            }
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    for (int i = 0; i < 5; ++i) {
        val = divisor_count(val);
    }
    return val;
}
