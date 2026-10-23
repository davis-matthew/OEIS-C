/*
 * A032839  Numbers whose set of base-15 digits is {3,4}.
 * https://oeis.org/A032839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032839: numbers whose base‑15 digits are only 3 or 4.
   The n‑th term is obtained by writing n‑1 as a binary number,
   interpreting each binary bit as a base‑15 digit (0 → 3, 1 → 4),
   and evaluating the resulting base‑15 number.        */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the length L of the base‑15 representation.
       L = floor(log2(n + 1)), i.e. the number of bits required to
       write n+1 minus one.  This gives the length of the number
       that contains the n‑th term.                           */
    idx_t temp = n + 1;
    idx_t L = 0;
    while (temp > 1) {
        temp >>= 1;
        L++;
    }

    /* Compute 2^L and the number of terms before the current length. */
    idx_t pow2 = 1ULL << L;           /* 2^L   (L ≥ 1 for n ≥ 1) */
    idx_t prev_cum = pow2 - 2;        /* cumulative terms up to length L-1 */

    /* Offset of n within the current length group (0 … 2^L-1).   */
    idx_t offset = n - prev_cum - 1;

    /* Convert the binary offset into the desired base‑15 digits. */
    seq_t result = 0;
    for (idx_t i = L; i > 0; i--) {
        /* Most significant bit first: (offset >> (i-1)) & 1  */
        seq_t digit = ((offset >> (i-1)) & 1) ? (seq_t)4 : (seq_t)3;
        result = result * 15 + digit;
    }
    return result;
}
