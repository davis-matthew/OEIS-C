/*
 * A115516  The mode of the bits of n (using 0 if bimodal).
 * https://oeis.org/A115516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The mode of the bits of n (using 0 if bimodal).
   a(n) = 1 if the number of 1-bits in the binary expansion of n is greater
   than the number of 0‑bits; otherwise a(n) = 0.  For n = 0 the binary
   representation is "0", so a(0) = 0. */
#include <stdint.h>

static inline int bitlen(idx_t n)
{
    if (n == 0) return 1;                    /* "0" has one bit          */
    return 64 - __builtin_clzll((unsigned long long)n);
}

/* Computes a(n) – the mode of the binary digits of n, with a tie
   favouring 0. */
seq_t a(idx_t n)
{
    unsigned long long u   = (unsigned long long)n;
    int               ones = __builtin_popcountll(u);
    int               len  = bitlen(n);          /* total number of bits   */
    /* zeros = len - ones; return 1 only if ones > zeros            */
    if ( (ones * 2) > len )
        return (seq_t)1;
    else
        return (seq_t)0;
}
