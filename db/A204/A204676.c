/*
 * A204676  a(n) = n! * (prime(n+1) + prime(n)) / (prime(n+1) - prime(n)).
 * https://oeis.org/A204676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (prime(n+1)+prime(n)) / (prime(n+1)-prime(n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for indices < 1 */

    seq_t fact = factorial(n);           /* n! */
    uint32_t p1 = get_prime(n);          /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);      /* prime(n+1) */

    seq_t numer = fact * (seq_t)(p1 + p2);
    seq_t denom = (seq_t)(p2 - p1);
    return numer / denom;
}
