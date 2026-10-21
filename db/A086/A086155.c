/*
 * A086155  a(n) is the number of primes between the primes p = A020483(n) and
 *          q = 2n + A020483(n).
 * https://oeis.org/A086155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #primes strictly between the least prime p with p+2n also
 *            prime and q = p+2n. That is, a(n) = |{p < r < p+2n : r prime}|
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* offset 1 – a(1) is the first listed term. */
    if (n == 0)
        return 0;

    idx_t d = 2 * n;          /* width of the interval */
    idx_t p = 2;              /* start with the first prime */

    /* Find the least prime p such that p and p+d are both primes. */
    while (1) {
        if (is_prime_small((seq_t)(p + d)))
            break;
        last:
        if (p == 2)
            p = 3;
        else
            p = next_prime((uint64_t)(p + 1));
    }

    /* Count primes strictly between p and q = p+d. */
    seq_t cnt = 0;
    for (idx_t m = p + 1; m < p + d; ++m) {
        if (is_prime_small((seq_t)m))
            ++cnt;
    }
    return cnt;
}
