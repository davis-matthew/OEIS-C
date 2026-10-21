/*
 * A077650  Initial decimal digit of sigma(n), the sum of divisors of n.
 * https://oeis.org/A077650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the first decimal digit of σ(n), the sum of divisors of n
 * Range:     u8 n <= 333, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(n) for a 64‑bit integer n and return it as a seq_t. */
static seq_t sigma_of_uint64(uint64_t n) {
    seq_t sigma = 1;
    uint64_t t = n;

    /* factor 2 */
    if ((t & 1ULL) == 0) {
        int e = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            e++;
        }
        seq_t term = 1;
        seq_t power = 1;
        for (int i = 0; i < e; ++i) {
            power *= 2;
            term += power;   /* 1 + 2 + 4 + ... + 2^e */
        }
        sigma *= term;
    }

    /* odd primes */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            seq_t term = 1;
            seq_t power = 1;
            for (int i = 0; i < e; ++i) {
                power *= p;
                term += power;   /* 1 + p + p^2 + ... + p^e */
            }
            sigma *= term;
        }
    }

    /* remaining prime factor */
    if (t > 1) {
        sigma *= (seq_t)(1 + t);
    }

    return sigma;
}

/* Return the first decimal digit of σ(n). */
seq_t a(idx_t n) {
    /* n is at least 1 according to the problem statement */
    seq_t s = sigma_of_uint64((uint64_t)n);

    /* strip trailing decimal digits until the most significant digit remains */
    while (s >= (seq_t)10) {
        s /= 10;
    }
    return s;
}
