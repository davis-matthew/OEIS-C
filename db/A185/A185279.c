/*
 * A185279  a(n) = number of ways that one can write n as the sum of two
 *          positive integers such that i) the integers are relatively prime
 *          to n but ii) the integers are not themselves prime.
 * https://oeis.org/A185279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ 1 ≤ i ≤ ⌊n/2⌋ : gcd(i, n-i)=1 ∧ i not prime ∧ (n-i)
 *            not prime }
 * Range:     u8 n <= 785, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        idx_t j = n - i;
        if (gcd((seq_t)i, (seq_t)j) != 1)
            continue;
        if (is_prime(i))
            continue;
        if (is_prime(j))
            continue;
        ++count;
    }
    return count;
}
