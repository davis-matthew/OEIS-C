/*
 * A074943  a(n) = tau(n) mod 3.
 * https://oeis.org/A074943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n) mod 3 – number of divisors of n taken modulo 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We compute the divisor‑count modulo 3 during factorisation
       to avoid large intermediate values.  The intermediate
       arithmetic is performed on standard unsigned 64‑bit
       integers; the final result (0,1,2) is cast to seq_t.  */
    uint64_t x = (uint64_t)n;   /* Safe conversion; n ≥ 1 */
    uint64_t res = 1;           /* tau(n) mod 3, initially 1 */

    /* Count factors of 2 */
    int exp = 0;
    while (x % 2ULL == 0) {
        exp++;
        x /= 2ULL;
    }
    if (exp > 0) {
        res = (res * (exp + 1ULL)) % 3U;
    }

    /* Count odd prime factors */
    for (uint64_t p = 3ULL; p * p <= x; p += 2ULL) {
        if (x % p == 0) {
            exp = 0;
            while (x % p == 0) {
                exp++;
                x /= p;
            }
            res = (res * (exp + 1ULL)) % 3U;
        }
    }

    /* If a prime factor larger than sqrt(x) remains */
    if (x > 1ULL) {
        res = (res * 2ULL) % 3U;    /* one prime factor of exponent 1 → (1+1)=2 */
    }

    return (seq_t)res;
}
