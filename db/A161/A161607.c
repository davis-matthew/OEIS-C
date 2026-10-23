/*
 * A161607  Positive integers k that are coprime to the value of the reversal
 *          of k's representation in binary.
 * https://oeis.org/A161607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A161607:
   positive integers k that are coprime to the reversal of k's binary representation. */

#include <stddef.h>
#include "../utils/gcd.h"

static seq_t rev_bin(seq_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;

    while (1) {
        seq_t r = rev_bin(k);
        if (gcd(k, r) == 1) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
