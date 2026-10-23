/*
 * A102632  Smallest k such that at least one of 2^k+/-prime(n) is prime.
 * https://oeis.org/A102632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 70, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): smallest k such that 2^k ± prime(n) is prime.
   n is 1‑based. Returns k as seq_t. */
#include <stdint.h>
#include "../utils/isprime_big.h"

static seq_t nth_prime(idx_t n)
{
    /* Simple incremental search for the nth prime, sufficient for n up to "
       about 10 000. Uses isprime_big which works for any seq_t. */
    if (n == 0) return 0;
    seq_t candidate = 2;
    idx_t count = 0;
    while (1)
    {
        if (isprime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* Find the nth prime (1‑based). */
    seq_t p = nth_prime(n);

    /* Start with 2^0 = 1. */
    seq_t s = 1;
    idx_t k = 0;

    for (;; ++k)
    {
        seq_t plus = s + p;
        if (isprime_big(plus))
            return (seq_t)k;

        if (s >= p)
        {
            seq_t minus = s - p;
            if (isprime_big(minus))
                return (seq_t)k;
        }

        /* Prepare the next power of two, checking for overflow. */
        seq_t next = s + s;            /* avoid shift overflow for arbitrary seq_t */
        if (next <= s)                 /* overflow occurred */
            break;
        s = next;
    }

    /* In practice this point should never be reached for the intended range
       of n. Return a large value to indicate failure. */
    return (seq_t)-1;
}
