/*
 * A039642  Number of steps to fixed point of "k -> k/2 or (k-1)/2 until
 *          result is prime", starting with prime(n)-1.
 * https://oeis.org/A039642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms (including the starting value) until
 *            reaching a prime, using the rule k -> k/2 if k is even,
 *            otherwise k -> (k-1)/2, starting from prime(n)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;
    if (x % 3 == 0)
        return x == 3;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0)
            return false;
        if (x % (i + 2) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* n-th prime (n >= 1) */
    uint64_t k = (uint64_t)p - 1;       /* starting value */
    seq_t steps = 1;                    /* count the starting term */

    for (;;) {
        if (k <= 1)                      /* reached 0 or 1 – stop */
            break;
        if (is_prime_uint64(k))          /* reached a prime – stop */
            break;

        /* compute the next value */
        uint64_t next = (k & 1ULL) ? ((k - 1ULL) >> 1) : (k >> 1);

        if (next == 0)                   /* avoid counting 0 */
            break;

        k = next;
        steps++;                          /* count this new term */
    }

    return steps;
}
