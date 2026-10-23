/*
 * A117058  Palindromes for which the product of the digits is prime.
 * https://oeis.org/A117058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117058: palindromes for which the product of the digits is prime.
   Each term is a palindrome of odd length where all digits are 1 except for
   a single prime digit (2, 3, 5, 7) in the centre.  The sequence is sorted
   in increasing numeric value.

   For n ≥ 1 let
       g   = (n‑1)/4                // group number, also (length‑1)/2
       p   = {2,3,5,7}[(n‑1)%4]      // centre prime digit
       left = (10^g – 1)/9           // g ones (0 when g = 0)

   Then
       a(n) = left * (10^(g+1) + 1) + p * 10^g
   All arithmetic is performed in seq_t. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence is 1‑based */

    idx_t k   = n - 1;              /* 0‑based index within the sequence */
    idx_t g   = k / 4;              /* group number = (length-1)/2 */
    idx_t pos = k % 4;              /* position within group */

    /* Prime digits allowed in the centre */
    static const seq_t primes[4] = {2, 3, 5, 7};
    seq_t center = primes[pos];

    /* Compute powers of 10 */
    seq_t pow10g  = pow10(g);          /* 10^g   */
    seq_t pow10g1 = pow10(g + 1);      /* 10^(g+1) */

    /* Number consisting of g ones: (10^g – 1)/9, zero when g=0 */
    seq_t left = 0;
    if (g > 0) {
        left = (pow10g - 1) / 9;
    }

    /* Final palindrome: left * (10^(g+1) + 1) + center * 10^g */
    seq_t palindrome = left * (pow10g1 + 1) + center * pow10g;
    return palindrome;
}
