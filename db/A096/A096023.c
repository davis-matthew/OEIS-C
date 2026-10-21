/*
 * A096023  Numbers congruent to {63, 123, 183, 243, 303, 363} mod 420.
 * https://oeis.org/A096023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096023: Numbers congruent to {63,123,183,243,303,363} mod 420.
 * Formula: a(n) = 420 * ((n-1)/6) + [63,123,183,243,303,363][(n-1)%6]   (n>=1)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t residues[6] = {63, 123, 183, 243, 303, 363};

    /* n is at least 1, so n-1 is safe */
    idx_t k = (n - 1) / 6;
    idx_t r = (n - 1) % 6;

    /* Multiply k by 420 using seq_t to avoid intermediate overflow in smaller types */
    seq_t term = (seq_t)k * 420 + residues[r];
    return term;
}
