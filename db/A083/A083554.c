/*
 * A083554  Least common multiple of prime(n+1)-1 and prime(n)-1.
 * https://oeis.org/A083554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(prime(n+1)-1, prime(n)-1) (n ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 74, u32 n <= 8952, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* seq_t get_prime(idx_t k) */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* prime indices are 1‑based in OEIS */
    seq_t p = get_prime(n);          /* prime(n)   */
    seq_t q = get_prime(n + 1);      /* prime(n+1) */

    seq_t p1 = p - (seq_t)1;         /* prime(n)   - 1 */
    seq_t q1 = q - (seq_t)1;         /* prime(n+1)- 1 */

    seq_t g = gcd(p1, q1);           /* gcd(prime(n)-1, prime(n+1)-1) */
    /* lcm = (p1 / g) * q1;   use division first to avoid overflow */
    seq_t result = (p1 / g) * q1;
    return result;
}
