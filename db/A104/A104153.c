/*
 * A104153  Numbers n such that n99 is prime and n is a multiple of ten.
 * https://oeis.org/A104153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n is a multiple of 10 and the decimal concatenation
 * n99 is prime.  a(n) is the n-th such number; a(1)=20. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_ull.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1; a(0) is unused */
    idx_t found = 0;
    uint64_t candidate = 10;                   /* first multiple of 10 */
    seq_t result = 0;

    while (found < n) {
        unsigned __int128 prod = (unsigned __int128)candidate * 100ULL + 99ULL;
        uint64_t test = (uint64_t)prod;        /* safe for the range of n requested */
        if (is_prime_ull(test)) {
            ++found;
            result = (seq_t)candidate;
        }
        candidate += 10;
    }
    return result;
}
