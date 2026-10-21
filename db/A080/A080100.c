/*
 * A080100  a(n) = 2^(number of 0's in binary representation of n).
 * https://oeis.org/A080100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^(number of 0's in binary representation of n)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Computes the n-th term of OEIS A080100.
 *  a(0) = 1, otherwise a(n) = 2^(#zeros in binary representation of n).
 */
seq_t a(idx_t n)
{
    /* Special case for n = 0: binary representation is "0" but we
     * treat it as having no zeros (following the sequence definition). */
    if (n == 0)
        return (seq_t)1;

    /* Count set bits and length of n. */
    unsigned long long val = (unsigned long long)n;
    unsigned pop   = __builtin_popcountll(val);
    unsigned bits  = 64 - __builtin_clzll(val);        /* number of bits of n */
    unsigned zeros = bits - pop;                      /* number of zeros */

    /* Return 2^zeros.  Shift is performed on a 64‑bit unsigned value
     * before casting to seq_t to avoid overflow for small types. */
    return (seq_t)(((unsigned long long)1) << zeros);
}
