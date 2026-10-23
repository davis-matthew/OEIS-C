/*
 * A121480  Take b(0)=n, for i>0 if b(i-1) is odd prime then b(i)=(b(i-1)+1)/2
 *          otherwise b(i)=b(i-1)+1. For all n sequence b(i) ends with 2. The
 *          sequence gives a(n) = a number of steps from n to 2, a(n)=i when
 *          b(i)=2.
 * https://oeis.org/A121480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps from n to 2 following the rule b(0)=n,
 *            for i>0 if b(i-1) is an odd prime then b(i)=(b(i-1)+1)/2 else
 *            b(i)=b(i-1)+1. a(n) is the first i with b(i)=2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;
    seq_t steps = 0;
    while (cur != 2) {
        if (is_prime_small(cur)) {
            cur = (cur + 1) / 2;
        } else {
            ++cur;
        }
        ++steps;
    }
    return steps;
}
