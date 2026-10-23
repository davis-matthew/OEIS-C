/*
 * A040864  Continued fraction for sqrt(894).
 * https://oeis.org/A040864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040864 (Continued fraction for sqrt(894))
   Multiplicative function with:
      a(0) = 29
      a(2) = 8
      a(2^e) = 58 for e ≥ 2
      a(p^e) = 1 for any odd prime p
   Since the function is multiplicative and all odd prime powers
   contribute 1, the value depends only on the exponent of 2 in n.
   Hence:
     • if n is odd (including 1)            → 1
     • if n ≡ 2 (mod 4) (exactly one factor 2) → 8
     • if n ≡ 0 (mod 4) (two or more factors 2)→ 58
   The formula is implemented directly. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)29;          /* special offset value   */
    if ((n & 1) == 0) {                   /* n even?                */
        if ((n & 3) == 0) return (seq_t)58;/* n has ≥2 factors of 2  */
        return (seq_t)8;                   /* n has exactly 1 factor */
    }
    return (seq_t)1;                       /* n odd (including 1)    */
}
