/*
 * A032071  Number of reversible strings with n labeled beads of 4 colors, no
 *          palindromes of more than 1 bead.
 * https://oeis.org/A032071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (4^n - 4^{ceil(n/2)}) / 2 for n ≥ 2; a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A032071: Number of reversible strings with n labelled beads of 4 colours,
    no palindromes of more than 1 bead. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 4;          /* a(1) = 4 */

    /* Compute 4^n and 4^{ceil(n/2)} */
    seq_t pow_n   = big_pow((seq_t)4, (seq_t)n);
    idx_t half    = (n + 1) / 2;               /* ceil(n/2) */
    seq_t pow_half = big_pow((seq_t)4, (seq_t)half);

    /* (4^n - 4^{ceil(n/2)}) / 2  */
    seq_t diff = pow_n - pow_half;
    seq_t half_diff = diff / 2;

    /* Multiply by n! */
    seq_t fact = factorial(n);
    return fact * half_diff;
}
