/*
 * A105339  a(n) = n*(n+1)/2 mod 1024.
 * https://oeis.org/A105339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 mod 1024
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the observation that n*(n+1) is always even.  
   We compute the product modulo 2048 (2×1024) prior to division by 2,
   so all intermediate values fit in 32 bits. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n and n+1 modulo 2048 to avoid overflow */
    uint64_t a_mod = ((uint64_t)n) & 2047ULL;           /* n mod 2048 */
    uint64_t b_mod = (((uint64_t)n + 1ULL) & 2047ULL);  /* (n+1) mod 2048 */

    /* product modulo 2048: use bitwise AND because 2048 = 2^11 */
    uint64_t prod = (a_mod * b_mod) & 2047ULL;          /* prod mod 2048 */

    /* divide by 2 (shift right) and convert to seq_t */
    return (seq_t)(prod >> 1);  /* result modulo 1024 */
}
