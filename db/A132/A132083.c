/*
 * A132083  a(n) = n-th positive integer which is coprime to (2^n +1). Also,
 *          a(n) = final term of row n in triangle A132082.
 * https://oeis.org/A132083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer coprime to (2^n + 1) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Compute the gcd of two uint64_t values. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the n-th positive integer coprime to (2^n + 1). */
seq_t a(idx_t n)
{
    /* Search sequentially for coprime candidates. */
    uint64_t count = 0;
    uint64_t candidate = 1;
    uint64_t N = (uint64_t)n;            /* cast once for efficiency */

    while (true)
    {
        /* r = (2^n + 1) mod candidate */
        seq_t r = (mod_pow((seq_t)2, N, (seq_t)candidate) + (seq_t)1) % (seq_t)candidate;
        uint64_t rem = (uint64_t)r;

        /* Check coprimality. */
        if (gcd_u64(candidate, rem) == 1)
        {
            ++count;
            if (count == N)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
