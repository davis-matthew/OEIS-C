/*
 * A189914  a(n) is 2^phi(n) times the least common multiple of the proper
 *          divisors of n.
 * https://oeis.org/A189914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n) * lcm_of_proper_divisors(n)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* phi(n) is Euler's totient function. For n = 1, phi(1) = 1. */

#include <stdint.h>
#include <stddef.h>

/* Integer square root (floor) for uint64_t numbers. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;          /* largest power of four <= max uint64 */
    while (bit > x) bit >>= 2;
    for (; bit; bit >>= 2) {
        uint64_t tmp = r + bit;
        if (tmp <= x) {
            x -= tmp;
            r = (r >> 1) | bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* Integer power: base^exp, both non‑negative.  Result is seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Compute Euler's totient phi(n) for n >= 1. */
static seq_t calc_phi(idx_t n)
{
    seq_t result = (seq_t)n;
    uint64_t tmp = (uint64_t)n;
    uint64_t root = isqrt_u64(tmp);
    for (uint64_t p = 2; p <= root && tmp > 1; ++p) {
        if (tmp % p == 0) {
            result -= result / (seq_t)p;
            while (tmp % p == 0) tmp /= p;
            root = isqrt_u64(tmp);
        }
    }
    if (tmp > 1) result -= result / (seq_t)tmp;   /* remaining prime factor */
    return result;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1. */
    if (n == 0) return (seq_t)1;

    /* Compute phi(n) */
    seq_t phi = calc_phi(n);

    /* Factor n to decide lcm of proper divisors. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    uint64_t m = (uint64_t)n;
    uint64_t root = isqrt_u64(m);

    for (uint64_t p = 2; p <= root && m > 1; ++p) {
        if (m % p == 0) {
            primes[pcnt] = p;
            exps[pcnt] = 0;
            while (m % p == 0) {
                m /= p;
                ++exps[pcnt];
            }
            ++pcnt;
            root = isqrt_u64(m);
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t lcm;
    if (pcnt <= 1) {                    /* n is 1 or a prime power */
        if (n == 1) {
            lcm = (seq_t)1;
        } else {
            /* lcm = p^(e-1) */
            lcm = pow_seq((seq_t)primes[0], (idx_t)(exps[0] - 1));
        }
    } else {
        /* n has at least two distinct prime factors -> lcm = n */
        lcm = (seq_t)n;
    }

    /* 2^phi(n) */
    seq_t twopow = pow_seq((seq_t)2, (idx_t)phi);

    /* Final result */
    return twopow * lcm;
}
