/*
 * A180115  A109613(n)-fold concatenation of A008619(n).
 * https://oeis.org/A180115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = ((n+1)/2) * (10^{n + (n%2==0)} - 1) / 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number a(n) consists of ⟨(n+1)/2⟩ repeated
   n + (n is even ? 1 : 0) times. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The digit to repeat (works for odd and even n) */
    seq_t digit = ((seq_t)n + 1) / 2;

    /* Length of the repeated digit block */
    idx_t len = n + ((n & 1) == 0);   /* Add 1 if n is even */

    /* 10^len – 1 */
    seq_t pow10_val = pow10(len);
    seq_t ones = pow10_val - 1;       /* (10^len - 1) */

    /* Repunit of length len: (10^len - 1) / 9 */
    seq_t repunit = ones / 9;

    /* Final result: digit * repunit */
    return digit * repunit;
}
