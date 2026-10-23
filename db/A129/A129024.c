/*
 * A129024  The first 8 values are predefined, the remaining set to a(n) =
 *          24*prime(n).
 * https://oeis.org/A129024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 3,15,24,48,72,120,168,264 for n = 1..8 and a(n) =
 *            24*prime(n) for n ≥ 9
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    static const seq_t first[9] = {
        0,
        (seq_t)3,
        (seq_t)15,
        (seq_t)24,
        (seq_t)48,
        (seq_t)72,
        (seq_t)120,
        (seq_t)168,
        (seq_t)264
    };

    if (n <= 8)
        return first[n];

    uint32_t p = get_prime(n);
    return (seq_t)p * ((seq_t)24);
}
