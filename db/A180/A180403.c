/*
 * A180403  Numerator in Moebius transform of A001790/A046161.
 * https://oeis.org/A180403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = Σ_{d|n} μ(d) * ( B(n/d) / 2^{v2(B(n/d))} ) * 2^{ 2n -
 *            2(n/d) + v2(B(n/d)) - v2(B(n)) }
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where B(m) = binomial(2m-2, m-1) and v2(B(m)) = 2*popcount(m-1) - popcount(2m-2). */
/* This implements the Möbius transform of A001790/A046161. */

#include <stdint.h>
#include <stdbool.h>

static inline int popcount64(uint64_t x) {
    return __builtin_popcountll(x);
}

static inline int v2_binom(uint64_t m) {
    /* v2 of binomial(2m-2, m-1) */
    int pc1 = popcount64(m - 1);
    int pc2 = popcount64((m << 1) - 2);
    return 2 * pc1 - pc2;
}

static seq_t binom_odd_part(uint64_t m) {
    seq_t res = 1;
    for (uint64_t i = 1; i < m; ++i) {
        res *= (seq_t)(m - 1 + i);
        res /= (seq_t)i;   /* exact division */
    }
    int v2 = v2_binom(m);
    res >>= v2;            /* remove all factors of 2 */
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 for safety */

    /* Factor n to obtain distinct primes */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[20];
    int pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1 && pcnt < 20) primes[pcnt++] = tmp;

    int v2_Bn = v2_binom((uint64_t)n);

    seq_t sum = 0;

    /* Iterate over all square‑free divisors via bitmask */
    unsigned total_masks = 1u << pcnt;
    for (unsigned mask = 0; mask < total_masks; ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (int i = 0; i < pcnt; ++i) {
            if (mask & (1u << i)) {
                d *= primes[i];
                ++bits;
            }
        }
        uint64_t m = n / d;           /* n/d */
        int v2_Bm = v2_binom(m);
        seq_t odd_part = binom_odd_part(m);

        /* Exponent for shifting left */
        long long exponent = (long long)2 * (long long)n
                            - (long long)2 * (long long)m
                            + (long long)v2_Bm
                            - (long long)v2_Bn;
        if (exponent < 0) exponent = 0;   /* safety, should not happen */

        seq_t term = odd_part << (unsigned long long)exponent;
        if (bits & 1) term = -term;       /* μ(d) = -1 for odd number of primes */

        sum += term;
    }

    return sum;
}
