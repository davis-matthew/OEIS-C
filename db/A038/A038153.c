/*
 * A038153  Beatty sequence for Pi^e.
 * https://oeis.org/A038153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * 22.459157718341055...)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rational approximation of π^e  */
#include <stdint.h>
static const uint64_t NUM = 2245915771834105500ULL;   /* floor(π^e * 10^17) */
static const uint64_t DEN = 100000000000000000ULL;     /* 10^17 */

/* Beatty sequence a(n) = floor(n * π^e) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, value for n==0 is unused */
    /* `unsigned __int128` is an extension available in GCC/Clang that
       guarantees 128‑bit arithmetic for the product.   */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    unsigned __int128 div  = prod / DEN;
    return (seq_t)div;
}
