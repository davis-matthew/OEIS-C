/*
 * A048050  Chowla's function: sum of divisors of n except for 1 and n.
 * https://oeis.org/A048050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = σ(n) - n - 1, i.e. sum of all divisors of n except 1 and
 *            n
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 or 2, there are no proper divisors except 1 and itself,
       thus a(n) = 0. */
    if (n <= 2)
        return 0;

    seq_t sum = 0;

    /* Enumerate all divisors i of n with 2 ≤ i ≤ sqrt(n). */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            /* Add i itself. */
            sum += (seq_t)i;

            /* Add the complementary divisor n/i if it is distinct
               from i and not equal to n (which never happens here
               because i ≥ 2). */
            idx_t j = n / i;
            if (j != i) {
                sum += (seq_t)j;
            }
        }
    }

    return sum;
}
