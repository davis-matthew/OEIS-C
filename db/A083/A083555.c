/*
 * A083555  Quotient of LCM of prime(n+1)-1 and prime(n)-1 and GCD of the same
 *          two numbers.
 * https://oeis.org/A083555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(prime(n+1)-1, prime(n)-1) / gcd(prime(n+1)-1,
 *            prime(n)-1)
 * Range:     u8 n <= 8, u16 n <= 96, u32 n <= 9363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n-th term of OEIS A083555 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* The offset of the sequence is 1, so n >= 1. */
    uint32_t p1 = get_prime(n);      /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);  /* prime(n+1) */

    seq_t x = (seq_t)p1 - (seq_t)1;  /* prime(n)   - 1 */
    seq_t y = (seq_t)p2 - (seq_t)1;  /* prime(n+1) - 1 */

    seq_t g   = gcd(x, y);          /* gcd of the two numbers */
    seq_t lcm = (x / g) * y;        /* lcm = x/g * y   (avoids overflow) */

    return lcm / g;                 /* result = lcm / gcd */
}
