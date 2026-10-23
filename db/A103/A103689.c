/*
 * A103689  a(n) is the least k such that either k*n - 1 or k*n + 1 (or both)
 *          is prime.
 * https://oeis.org/A103689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k ≥ 1 | is_prime(k*n-1) or is_prime(k*n+1)}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Indexing starts at 1; for n == 0 the value is undefined but
       we return 0 to avoid UB.  */
    if (n == 0)
        return 0;

    /* Work with 64‑bit arithmetic for the search;  n is small enough
       that k*n fits in 64 bits for the first prime found.  */
    uint64_t nn = (uint64_t)n;

    for (uint64_t k = 1;; ++k)
    {
        unsigned __int128 prod = (unsigned __int128)k * nn;
        uint64_t val1 = (uint64_t)prod - 1;
        if (val1 >= 2 && is_prime(val1))
            return (seq_t)k;

        uint64_t val2 = (uint64_t)prod + 1;
        if (val2 >= 2 && is_prime(val2))
            return (seq_t)k;
    }
}
