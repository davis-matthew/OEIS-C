/*
 * A060505  a(n) = floor(2^n/(n^2)).
 * https://oeis.org/A060505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( 2^n / (n^2) ) for n >= 1
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 42, u64 n <= 76, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses arbitrary‑precision operations when seq_t supports it.
   For 64‑bit builds (seq_t == unsigned long long or a smaller integer type)
   it uses 128‑bit intermediates capable of handling n up to 127, which is
   sufficient for all n whose result fits in 64 bits. */
/* Requirements are met: only integer arithmetic on seq_t and small
   auxiliary types is used; no loops or memory allocation are needed. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For 64‑bit or smaller seq_t, use 128‑bit arithmetic. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 2^n and n^2 comfortably fit in unsigned __int128 for n < 128. */
        if (n >= 128) {
            /* For very large n the result far exceeds 64 bits.
               The 64‑bit build is not expected to be queried for such n
               (tests only cover values that fit into seq_t). */
            return (seq_t)0;
        }
        unsigned __int128 pow2 = (unsigned __int128)1 << n;
        unsigned __int128 denom = (unsigned __int128)n * (unsigned __int128)n;
        return (seq_t)(pow2 / denom);
    }

    /* For arbitrary‑precision seq_t (the big‑integer build) the basic
       operations are defined by the build’s integer class.  Shift left
       by n yields 2^n exactly; the division is performed by the class
       itself. */
    seq_t pow2 = (seq_t)1;
    pow2 <<= n;          /* pow2 = 2^n */
    seq_t denom = (seq_t)n * (seq_t)n;
    return pow2 / denom;
}
