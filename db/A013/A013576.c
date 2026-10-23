/*
 * A013576  Minimal scope of an (n,4) difference triangle.
 * https://oeis.org/A013576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal scope of an (n,4) difference triangle
   A013576 = 11, 22, 32, 41, 51, 60, 71, 80, 91, 100, 111, 120, ...
   Formula: a(1)=11; a(2)=22; a(3)=32; a(4)=41;
            a(n)=10*n + (n%2)   for n >= 5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four exceptional values explicitly. */
    if (n == 1u) return (seq_t)11;
    if (n == 2u) return (seq_t)22;
    if (n == 3u) return (seq_t)32;
    if (n == 4u) return (seq_t)41;

    /* For n >= 5: a(n) = 10*n + (n%2).  Compute in seq_t. */
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t val = sn * (seq_t)10;
    val += ((n & 1u) ? (seq_t)1 : (seq_t)0);
    return val;
}
