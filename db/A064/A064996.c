/*
 * A064996  A Beatty sequence: a(n) = floor(Pi^2 - 8).
 * https://oeis.org/A064996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n * (π² – 8) ⌋ (Beatty sequence)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Pre‑computed scaling:  
   (π² – 8) ≈ 1.869604401089358...       
   NUM = ⌊(π² – 8) · 10¹²⌋ = 1869604401089.
   SCALE = 10¹².  */
static const unsigned long long NUM   = 1869604401089ULL;
static const unsigned long long SCALE = 1000000000000ULL;

seq_t a(idx_t n)
{
    /* n · NUM may exceed 64 bits for large n; use 128‑bit integer
       arithmetic to avoid overflow, then truncate by SCALE.  */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    return (seq_t)(prod / SCALE);
}
