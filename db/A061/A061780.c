/*
 * A061780  Number of solutions to x + y + z = 0 mod (2n+1) such that x,y,z
 *          are units modulo 2n+1, i.e., gcd(x, 2n+1) = gcd(y, 2n+1) = gcd(z,
 *          2n+1) = 1.
 * https://oeis.org/A061780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (2n+1)^2 * Π_{p|2n+1} (1 - 3/p + 2/p^2)
 * Range:     u8 n <= 8, u16 n <= 129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 2n+1 = m, the product is iterated over the distinct prime divisors of m. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For safety return 0 for n = 0. */
    if (n == 0) return 0;

    uint64_t m = 2 * n + 1;
    seq_t result = (seq_t)m * (seq_t)m;   /* (2n+1)^2 */

    /* Factorise m and collect its distinct primes. */
    uint64_t temp = m;
    uint64_t primes[64];
    size_t pcnt = 0;

    if (temp % 2 == 0) {
        primes[pcnt++] = 2;
        while (temp % 2 == 0) temp /= 2;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[pcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[pcnt++] = temp;

    /* Apply the product factor for each prime p: multiply by (p^2-3p+2)/p^2. */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        seq_t p_seq = (seq_t)p;
        seq_t divisor = p_seq * p_seq;                     /* p^2 */
        result = result / divisor;                         /* exact division */
        seq_t factor = p_seq * p_seq - (seq_t)3 * p_seq + (seq_t)2;  /* p^2-3p+2 */
        result = result * factor;                          /* integer multiplication */
    }

    return result;
}
