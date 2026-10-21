/*
 * A200815  Number of iterations of k -> d(k) until n reaches an odd prime.
 * https://oeis.org/A200815
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 548, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computes the number of τ-iterations of k → τ(k) until an odd prime is reached. */

static seq_t tau(idx_t n)
{
    seq_t result = 1;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }
    if (t > 1)            /* remaining prime factor */
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    seq_t count = 0;

    for (;;)
    {
        /* Stop when k is an odd prime */
        if (k > (seq_t)2 && (k & (seq_t)1) && is_prime((idx_t)k))
            break;

        /* Apply τ(k) */
        idx_t k64 = (idx_t)k;              /* k fits into idx_t for all n <= 10000 */
        seq_t t = tau(k64);
        k = t;
        ++count;
    }
    return count;
}
