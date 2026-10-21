/*
 * A070293  a(n) = lcm(30,n)/gcd(30,n).
 * https://oeis.org/A070293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(30,n)/gcd(30,n) = (30/gcd(30,n)) * (n/gcd(30,n))
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small Euclidean gcd for 64‑bit values.  The result is ≤ 30. */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Convert index to a 64‑bit integer for gcd calculation. */
    uint64_t nn = (uint64_t)n;
    /* Compute g = gcd(30, n).  g fits in a 64‑bit integer. */
    uint64_t g = gcd_uint64(30U, nn);
    /* Compute the two integer factors.  All intermediate results stay within
       64‑bit range because 30/g ≤ 30 and nn/g ≤ n. */
    seq_t part1 = (seq_t)(30U / g);
    seq_t part2 = (seq_t)(nn / g);
    /* Return the product as the desired sequence value. */
    return part1 * part2;
}
