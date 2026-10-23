/*
 * A163506  a(n) = n-th odd nonprime * n-th odd number.
 * https://oeis.org/A163506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014076(n) * (2*n-1)
 * Range:     u8 n <= 5, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A014076(n) is the nth odd nonprime number. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t nth_odd_nonprime(idx_t n)
{
    uint64_t count = 0;
    uint64_t num   = 1;        /* start at 1, the first odd nonprime */
    while (true)
    {
        if (num == 1 || !is_prime(num))
        {
            ++count;
            if (count == n)
                return num;
        }
        num += 2;              /* consider only odd numbers */
    }
}

seq_t a(idx_t n)
{
    uint64_t odd_nonprime = nth_odd_nonprime(n);
    seq_t    odd_num = ((seq_t)n << 1) - 1;   /* (2*n-1) */
    return (seq_t)odd_nonprime * odd_num;
}
