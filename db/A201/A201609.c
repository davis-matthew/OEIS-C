/*
 * A201609  Primes of the form 7n^2 + 9.
 * https://oeis.org/A201609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 1894, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrime64, mul64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 7*n^2 + 9 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>      /* for size_t */
#include "../utils/isPrime64.h"
#include "../utils/mul64.h"
#include "../utils/powmod64.h"

/* Return the n-th prime of the form 7*k^2 + 9, offset = 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for offsets < 1 */
    idx_t found = 0;                    /* how many primes found */
    for (idx_t k = 1; ; ++k)
    {
        /* compute p = 7*k^2 + 9; k^2 fits in 64 bits for requested ranges */
        uint64_t kk = (uint64_t)k;
        uint64_t p = 7ULL * kk * kk + 9ULL;
        if (isPrime64(p))
        {
            ++found;
            if (found == n) return (seq_t)p;
        }
    }
}
