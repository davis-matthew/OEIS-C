/*
 * A160995  The smallest positive integer neither a divisor of n nor coprime
 *          to n.
 * https://oeis.org/A160995
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that * gcd(m,n) > 1 and
 *            m does not divide n. For odd n, * a(n) = 2 * smallest prime
 *            factor of n. * For even n, a(n) = min{p^(e+1) | p^e || n} ∪
 *            {2*q}, * where q is the smallest odd prime not dividing n
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small power routine that works for any seq_t */
static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* trial‑division primality test for uint64_t, sufficient for small primes */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* return the next odd prime >= start (start >= 3) */
static uint64_t next_prime_from(uint64_t start) {
    if (start <= 2) return 3;
    if ((start & 1) == 0) start++;
    while (!is_prime_uint64(start)) start += 2;
    return start;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* undefined for n < 2 */

    /* factor n by trial division; store smallest prime factor and
       compute candidate p^(e+1) for each prime power. */
    uint64_t temp = (uint64_t)n;
    uint64_t smallest_prime_factor = 0;
    seq_t min_candidate = 0;      /* 0 means "not set yet" */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            do { temp /= p; exp++; } while (temp % p == 0);
            seq_t cand = seq_pow_seq((seq_t)p, exp + 1);
            if (min_candidate == 0 || cand < min_candidate)
                min_candidate = cand;
            if (smallest_prime_factor == 0) smallest_prime_factor = p;
        }
    }
    if (temp > 1) {               /* last prime factor */
        seq_t cand = seq_pow_seq((seq_t)temp, 2);
        if (min_candidate == 0 || cand < min_candidate)
            min_candidate = cand;
        if (smallest_prime_factor == 0) smallest_prime_factor = temp;
    }

    /* odd n: answer is simply 2 * smallest prime factor */
    if (n & 1) {
        return (seq_t)2 * (seq_t)smallest_prime_factor;
    }

    /* even n: add the 2*q candidate, where q is the smallest odd prime not dividing n */
    uint64_t q = next_prime_from(3);
    while (n % q == 0) q = next_prime_from(q + 2);   /* skip primes that divide n */

    seq_t cand_even = 2 * (seq_t)q;
    if (cand_even < min_candidate) min_candidate = cand_even;

    return min_candidate;
}
