/*
 * A011680  A binary m-sequence: expansion of reciprocal of
 *          x^7+x^6+x^5+x^4+x^2+x+1.
 * https://oeis.org/A011680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements the binary m‑sequence produced by the primitive
   polynomial x^7+x^6+x^5+x^4+x^2+x+1.  The sequence is defined by
   a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1 and for n≥7

   a(n) = a(n-7) XOR a(n-6) XOR a(n-5) XOR a(n-4) XOR a(n-2) XOR a(n-1).

   The sequence is 127‑periodic. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of 127 values */
    static seq_t table[127];
    static int initialized = 0;

    if (!initialized) {
        /* initial values */
        for (idx_t i = 0; i < 7; ++i)
            table[i] = 0;
        table[6] = 1;
        /* compute remaining 120 values */
        for (idx_t i = 7; i < 127; ++i) {
            table[i] =
                table[i-7] ^ table[i-6] ^ table[i-5] ^ table[i-4] ^
                table[i-2] ^ table[i-1];
        }
        initialized = 1;
    }

    /* 127‑periodicity */
    idx_t idx = n % 127;
    return table[idx];
}
