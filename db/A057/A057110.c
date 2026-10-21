/*
 * A057110  Difference between largest prime power factor of n and the
 *          smallest number S(n) with S(n)! a multiple of n [taking a(1)=0].
 * https://oeis.org/A057110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A034699(n) - A002034(n) * A034699(n) = largest
 *            prime‑power factor of n * A002034(n) = smallest m such that m!
 *            is a multiple of n * a(1) = 0 * * The function computes these
 *            two values by trial division of n * and by Legendre’s formula to
 *            find the minimal factorial index * for each prime power
 * Range:     u8 n <= 342, u16 n <= 9226, u32 n <= 9226, u64 n <= 9226,
 *            u128 n <= 9226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Legendre: exponent of prime p in m! */
static inline unsigned int
v_p_factorial(uint64_t m, uint64_t p)
{
    unsigned int cnt = 0;
    while (m >= p) {
        m /= p;
        cnt += (unsigned int)m;
    }
    return cnt;
}

/* Minimal m with vp(m!) >= e.  Uses a binary search. */
static inline uint64_t
min_m_for_prime_power(uint64_t p, uint64_t e)
{
    uint64_t low = 1, high = p * e;
    if (high < 2) high = 2;

    /* Increase high until it suffices. */
    while (v_p_factorial(high, p) < e) high <<= 1;

    /* Binary search for the smallest adequate m. */
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (v_p_factorial(mid, p) >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;           /* a(1) = 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t max_prime_power = 1;    /* largest prime‑power factor */
    uint64_t min_factorial_index = 0;/* minimal m with m! % n == 0 */

    uint64_t temp = nn;

    /* Factorise n. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            uint64_t pp = 1;
            while (temp % p == 0) {
                temp /= p;
                ++e;
                pp *= p;
            }
            if (pp > max_prime_power) max_prime_power = pp;

            uint64_t m = min_m_for_prime_power(p, e);
            if (min_factorial_index == 0 || m > min_factorial_index)
                min_factorial_index = m;
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        uint64_t p   = temp;
        uint64_t e   = 1;
        uint64_t pp  = p;
        if (pp > max_prime_power) max_prime_power = pp;

        uint64_t m = min_m_for_prime_power(p, e);
        if (min_factorial_index == 0 || m > min_factorial_index)
            min_factorial_index = m;
    }

    /* Difference between largest prime power and minimal factorial index. */
    return (seq_t)max_prime_power - (seq_t)min_factorial_index;
}
