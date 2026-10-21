/*
 * A077135  Composite numbers n whose proper (other than 1 and n) odd divisors
 *          are prime and even divisors are 1 less than a prime.
 * https://oeis.org/A077135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite number whose proper odd divisors are prime
 *            * and whose proper even divisors are one less than a prime. *
 *            The function computes this by enumerating integers in increasing
 *            order * and counting only the numbers that satisfy the condition
 * Range:     u8 n <= 90, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* helper: primality test for 64‑bit integers, using trial division  */
static inline bool is_prime_u64(uint64_t v)
{
    if (v < 2)          return false;
    if (v % 2 == 0)     return v == 2;
    for (uint64_t d = 3; d <= v / d; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Compute the n-th term of the sequence (n ≥ 1).                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, offset is 1 */

    idx_t   count = 0;                    /* how many valid terms found */
    uint64_t m    = 4;                    /* start from the first composite */

    while (true) {
        /* skip primes outright */
        if (is_prime_u64(m)) { m++; continue; }

        /* find a non‑trivial factor */
        uint64_t sp = 0;
        for (uint64_t d = 2; d <= m / d; d += (d == 2 ? 1 : 2)) {
            if (m % d == 0) { sp = d; break; }
        }
        if (sp == 0) { m++; continue; }             /* should not happen */

        uint64_t q = m / sp;

        /* semiprime test: m = p * q with both prime */
        if (is_prime_u64(q)) {
            ++count;
            if ((idx_t)count == n) return (seq_t)m;
        }
        /* 4p test: m divisible by 4 and p, 2p+1 are prime */
        else if ((m & 0x3) == 0) {                   /* 4 divides m */
            uint64_t p = m >> 2;                     /* m / 4 */
            if (is_prime_u64(p) && is_prime_u64(2 * p + 1)) {
                ++count;
                if ((idx_t)count == n) return (seq_t)m;
            }
        }
        m++;
    }
}
