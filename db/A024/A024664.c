/*
 * A024664  n written in fractional base 10/9.
 * https://oeis.org/A024664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 89, u64 n <= 329,
 *            u128 n <= 2729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n written in fractional base 10/9.
 * For base a/b, if a digit d exceeds a-1, subtract a and carry b.
 * The algorithm below repeatedly extracts the least significant digit
 * and carries over to the next position: 
 *   d  = n mod 10;
 *   n  = ((n - d) / 10) * 9;
 * The digits collected (least‑to‑most significant) are then reversed
 * to form the final integer value.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Special case: the first term of the sequence. */
    if (n == 0)
        return 0;

    /* Working copy of n in seq_t; this is where the carry operations are done. */
    seq_t temp = (seq_t)n;

    /* Digits of the result in least‑to‑most significant order.
     * For n <= 2^64-1 the representation needs at most about 450 digits,
     * 1024 is comfortably safe even for bigger arbitrary‑precision seq_t. */
    seq_t digits[1024];
    size_t cnt = 0;

    while (temp != 0)
    {
        /* The current least significant digit (0 <= d < 10). */
        seq_t d = temp % (seq_t)10;

        /* Update temp for the next round: subtract the digit,
           divide by a=10, and carry a multiple of b=9. */
        temp = ((temp - d) / (seq_t)10) * (seq_t)9;

        digits[cnt++] = d;
    }

    /* Reconstruct the integer from the most significant digit downwards. */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        result = result * (seq_t)10 + digits[cnt - 1 - i];
    }

    return result;
}
