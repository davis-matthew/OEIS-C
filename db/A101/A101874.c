/*
 * A101874  Number of Abelian groups of order 4n+3.
 * https://oeis.org/A101874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{p|m} p(e_p) with m = 4*n+3 where e_p is the exponent
 *            of p in the prime factorisation of m and p(k) is the partition
 *            number of k (number of partitions of k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Pre‑computed partition numbers p(k) for k = 0..26.
   p(k) = number of integer partitions of k. */
static const seq_t PART_MAX[] = {
    1,      /* p(0) */
    1,      /* p(1) */
    2,      /* p(2) */
    3,      /* p(3) */
    5,      /* p(4) */
    7,      /* p(5) */
    11,     /* p(6) */
    15,     /* p(7) */
    22,     /* p(8) */
    30,     /* p(9) */
    42,     /* p(10) */
    56,     /* p(11) */
    77,     /* p(12) */
    101,    /* p(13) */
    135,    /* p(14) */
    176,    /* p(15) */
    231,    /* p(16) */
    297,    /* p(17) */
    385,    /* p(18) */
    490,    /* p(19) */
    627,    /* p(20) */
    792,    /* p(21) */
    1001,   /* p(22) */
    1255,   /* p(23) */
    1575,   /* p(24) */
    1958,   /* p(25) */
    2436    /* p(26) */
};

/* Compute the number of Abelian groups of order 4n+3. */
seq_t a(idx_t n) {
    /* Compute m = 4*n + 3.  Use 64‑bit arithmetic; for n in the
       typical test range m comfortably fits in uint64_t. */
    uint64_t m = 4 * n + 3ULL;

    seq_t result = 1;
    uint64_t x = m;

    /* Trial division: factor m into primes and accumulate the
       product of partition numbers of the exponents. */
    for (uint64_t p = 3; p <= x / p; p += 2) {   /* only odd primes needed */
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            /* exp is small (≤9 for m ≈ 40000), so PART_MAX[exp] exists */
            result *= PART_MAX[exp];
        }
    }

    /* If a prime factor > sqrt(m) remains, its exponent is 1. */
    if (x > 1) {
        result *= PART_MAX[1];   /* p(1) = 1, multiplies unchanged */
    }

    return result;
}
