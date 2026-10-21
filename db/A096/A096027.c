/*
 * A096027  Numbers k such that (k+j) mod (2+j) = 1 for j from 0 to 10 and
 *          (k+11) mod 13 <> 1.
 * https://oeis.org/A096027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096027: Numbers k such that k ≡ 3 (mod 27720) but k ≠ 3 (mod 360360).
   Formula: a(n) = 3 + 27720 * ( n + (n-1)/12 )  for n ≥ 1.
   The term a(n) corresponds to the integer k = 3 + 27720 * m where
   m = n + ⌊(n-1)/12⌋ and m is not a multiple of 13.
   Complexity: O(1). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset < 1 */
    /* Compute m = n + floor((n-1)/12) without overflow */
    unsigned __int128 m = (unsigned __int128)n + ((unsigned __int128)(n - 1) / 12);
    /* Compute a(n) = 3 + 27720 * m */
    unsigned __int128 val = (unsigned __int128)27720 * m + 3;
    return (seq_t)val;
}
