/*
 * A129574  Number of odd divisors of n plus the number of odd divisors of n -
 *          1.
 * https://oeis.org/A129574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001227(n) + A001227(n-1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count odd divisors of n.  n is 0 => return 0. */
static seq_t odd_divisor_count(idx_t n)
{
    if (n == 0)
        return 0;

    /* Remove all factors of 2 */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    seq_t count = 1;
    idx_t temp = odd;

    /* Trial factorisation of odd part */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            count *= (exp + 1);
        }
    }

    /* If a prime > sqrt remains */
    if (temp > 1)
        count *= 2;

    return count;
}

seq_t a(idx_t n)
{
    return odd_divisor_count(n) + odd_divisor_count(n - 1);
}
