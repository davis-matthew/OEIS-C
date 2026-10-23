/*
 * A102611  Numbers k such that the number k77 is prime.
 * https://oeis.org/A102611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the n-th number k such that the decimal number k77 = 100*k+77 is
 *            prime
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for k in increasing order and returns the n-th such k. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 as a placeholder */

    idx_t count = 0;
    uint64_t k = 1;                 /* candidate k */

    while (1)
    {
        /* Skip obvious composite cases:
           100*k + 77 is divisible by 3 if k ≡ 1 (mod 3)
           divisible by 7 if k ≡ 0 (mod 7)
           divisible by 11 if k ≡ 0 (mod 11)
        */
        if (k % 3 == 1) { ++k; continue; }
        if (k % 7 == 0) { ++k; continue; }
        if (k % 11 == 0) { ++k; continue; }

        uint64_t val = 100 * k + 77;    /* k77 as a uint64_t */
        if (is_prime(val))
        {
            ++count;
            if (count == n)
                return (seq_t)k;        /* found the n-th term */
        }
        ++k;
    }
}
