/*
 * A137922  Numbers having exactly two divisors d such that d+1 is not a
 *          divisor.
 * https://oeis.org/A137922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137922: Numbers having exactly two divisors d such that d+1 is not a divisor.
   For n <= 4: a(1)=3, a(2)=4, a(3)=5, a(4)=6.
   For n > 4: a(n) = prime_{n-1} (the (n-1)-th prime). */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return 3;
    if (n == 2) return 4;
    if (n == 3) return 5;
    if (n == 4) return 6;
    /* n >= 5 */
    return (seq_t)get_prime(n - 1);
}
