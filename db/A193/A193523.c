/*
 * A193523  Number of odd divisors of Sopf(n).
 * https://oeis.org/A193523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4603, u16 n <= 9713, u32 n <= 9713, u64 n <= 9713,
 *            u128 n <= 9713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of odd divisors of Sopf(n),
   where Sopf(n) is the sum of distinct primes dividing n.
   a(1) = 0; for n > 1, a(n) = A001227(Sopf(n)).
*/
#include <stdint.h>

static seq_t distinct_prime_sum(idx_t n)
{
    uint64_t t = (uint64_t)n;
    seq_t sum = 0;
    if (t < 2) return 0;
    /* handle factor 2 */
    if (t % 2 == 0) {
        sum += 2;
        while (t % 2 == 0) t /= 2;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            sum += (seq_t)p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) sum += (seq_t)t;
    return sum;
}

static seq_t odd_divisor_count(seq_t x)
{
    if (x <= 1) return (x == 1) ? (seq_t)1 : (seq_t)0; /* 1 has 1 odd divisor, 0 handled by caller */
    while ((x & 1) == 0) x >>= 1;          /* extract odd part */
    if (x == 1) return (seq_t)1;           /* only 1 remains */
    seq_t cnt = 1;
    uint64_t t = (uint64_t)x;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            cnt *= (seq_t)(e + 1);
        }
    }
    if (t > 1) cnt *= 2;                 /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t sopf = distinct_prime_sum(n);
    return odd_divisor_count(sopf);
}
