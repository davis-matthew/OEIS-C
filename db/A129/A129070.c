/*
 * A129070  Numbers k such that (k-5)/4 is prime.
 * https://oeis.org/A129070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*prime(n) + 5
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function get_prime from utils */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset does not matter for n < 1 */
    seq_t p = (seq_t)get_prime(n);
    return p * (seq_t)4 + (seq_t)5;
}
