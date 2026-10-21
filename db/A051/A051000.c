/*
 * A051000  Sum of cubes of odd divisors of n.
 * https://oeis.org/A051000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1604, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d | n, d odd} d^3
 *
 * The sequence is multiplicative and
 * for an odd prime power p^e,
 * a(p^e) = (p^(3e+3)-1)/(p^3-1) = 1 + p^3 + p^6 + ... + p^(3e).
 * For n = 2^k * m with m odd, a(n) = a(m).  In particular a(2^k)=1.
 *
 * The implementation factors the odd part of n by trial division,
 * then multiplies the contributions from each odd prime power.
 * No intermediate values exceed the obvious bounds of the result,
 * and the routine works for any integer type seq_t that supports
 * +, -, *, /, % and comparison operators.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: integer power for seq_t with exponent as idx_t */
static seq_t pow_int_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* remove all factors of 2 */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        if (m == 0)        /* n==0 would never happen in the sequence */
            return 0;
    }
    if (m == 1)
        return 1;           /* n is a power of 2 */

    seq_t res = 1;
    uint64_t temp = m;      /* 64‑bit integer for factorisation */

    /* trial division over odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* compute 1 + p^3 + p^6 + ... + p^(3e) */
            seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
            seq_t sum = 1;
            seq_t cur = p3;
            for (uint64_t i = 0; i < e; ++i) {
                sum += cur;
                cur = cur * p3;
            }
            res *= sum;
        }
    }

    /* remaining prime factor if any */
    if (temp > 1) {
        uint64_t e = 1;
        seq_t p3 = (seq_t)temp * (seq_t)temp * (seq_t)temp;
        seq_t sum = 1;
        seq_t cur = p3;            /* p^(3*1) = p^3 */
        for (uint64_t i = 0; i < e; ++i) {
            sum += cur;
            cur = cur * p3;        /* next multiple of p^3 */
        }
        res *= sum;
    }

    return res;
}
