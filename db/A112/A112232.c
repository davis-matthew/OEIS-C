/*
 * A112232  Repeat each composite number in the sequence of natural numbers.
 * https://oeis.org/A112232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = repeat each composite number in the sequence of natural
 *            numbers. Implementation: iterate integers, adding 1 for primes
 *            (and 1) and 2 for composites until the running total reaches n;
 *            return the current integer
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the first term explicitly */
    if (n <= 1)
        return 1;

    idx_t m = 1;         /* current integer */
    idx_t count = 1;     /* cumulative number of terms up to m */

    for (m = 2;; ++m)
    {
        bool prime = is_prime((uint64_t)m);
        count += prime ? 1 : 2;   /* 1 for primes/1, 2 for composites */

        if (count >= n)
            return (seq_t)m;     /* sufficient terms reached */
    }
}
