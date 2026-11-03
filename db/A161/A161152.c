/*
 * A161152  Positive integers n such that {the number of (non-leading) 0's in
 *          the binary representation of n} is coprime to n.
 * https://oeis.org/A161152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer whose number of non‑leading 0
 *            bits in its binary representation is coprime to the integer
 *            itself
 * Range:     u8 n <= 162, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 982 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Count the number of zeros after the most‑significant 1 bit
   in the binary representation of x (x >= 1). */
static inline unsigned int nonleading_zeros(idx_t x)
{
    unsigned long long ux = (unsigned long long)x;
    unsigned int bits   = 64 - __builtin_clzll(ux);          /* number of bits used */
    unsigned int ones   = __builtin_popcountll(ux);          /* number of 1 bits    */
    return bits - 1 - (ones - 1); /* zeros after the MSB */
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; sentinel value */

    idx_t   count     = 0;
    idx_t   candidate = 1;

    while (1) {
        unsigned int z = nonleading_zeros(candidate);
        seq_t g = gcd((seq_t)z, (seq_t)candidate);  /* gcd(z, candidate) */
        if (g == 1) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
