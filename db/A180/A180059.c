/*
 * A180059  Smallest k such that binary weight of k*n is less than binary
 *          weight of k, or zero if no such k exists (for n = powers of 2).
 * https://oeis.org/A180059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive k such that popcount(k*n) <
 *            popcount(k), or 0 if none
 * Range:     u8 n <= 10, u16 n <= 636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Here n is a positive integer; if n is a power of 2, the function returns 0.) */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

/* Count the number of 1 bits in a seq_t value. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* No such k exists for powers of two. */
    if (is_power_of_two(N)) return 0;

    /* Brute‑force search for the smallest k.  The bound is chosen large enough
       for all n up to about 10 000.  In practice the answer is found far
       before reaching this bound.  If it does not exist the function
       returns 0. */
    for (idx_t k = 1; k <= 1000000; ++k)
    {
        seq_t K   = (seq_t)k;
        seq_t prod = K * N;
        if (popcount_seq_t(prod) < popcount_seq_t(K))
            return K;
    }

    return 0;
}
