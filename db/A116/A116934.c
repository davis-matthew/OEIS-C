/*
 * A116934  First prime in the arithmetic progression (n+k*p: k>0), where p is
 *          the smallest coprime greater than n+1.
 * https://oeis.org/A116934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1868, u32 n <= 5151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): first prime in arithmetic progression n + k*p, k>0,
   where p is the smallest integer > n+1 coprime to n. This implementation
   searches the progression directly. */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Find the smallest integer p > n+1 that is coprime to n */
    seq_t gn = (seq_t)n;
    seq_t p  = gn + (seq_t)2;        /* start just above n+1 */
    while (gcd(p, gn) != (seq_t)1) {
        p++;
    }

    /* Search the arithmetic progression n + k*p, k > 0, for the first prime */
    seq_t k = (seq_t)1;
    while (1) {
        seq_t candidate = gn + k * p;
        if (is_prime_big(candidate)) {
            return candidate;
        }
        k++;
    }
}
