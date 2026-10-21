/*
 * A059815  Let g_n be the ball packing n-width for the manifold torus X
 *          square; sequence gives numerator of (g_n/Pi)^2.
 * https://oeis.org/A059815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059815: a(n) = (n % 2 == 0) ? 1 : 2 for n >= 8; otherwise a(1..7) = 1,1,4,4,9,16,64. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first seven explicit terms */
    switch (n) {
    case 1: return (seq_t)1;
    case 2: return (seq_t)1;
    case 3: return (seq_t)4;
    case 4: return (seq_t)4;
    case 5: return (seq_t)9;
    case 6: return (seq_t)16;
    case 7: return (seq_t)64;
    default:
        /* For n >= 8 the sequence alternates 1 (even) , 2 (odd) */
        return ((n & 1) == 0) ? (seq_t)1 : (seq_t)2;
    }
}
