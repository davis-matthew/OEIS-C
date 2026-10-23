/*
 * A157226  Number of primitive inequivalent sublattices of square lattice
 *          having mirrors parallel to the sides of the unit cell of the
 *          parent lattice of index n.
 * https://oeis.org/A157226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) for odd n, a(n) = b(n)+b(n/2) for even n, where b(n)
 *            = 2^(ω(n)-1) for n>1 (ω(n) = number of distinct prime factors),
 *            and a(1) = 0
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static idx_t spf_cap = 0;
static uint32_t *spf = NULL;

/* Ensure smallest prime factor array is computed up to max_n. */
static void ensure_spf(idx_t max_n) {
    if (max_n < 2) max_n = 2;
    if ((size_t)max_n + 1 <= spf_cap) return;   /* already computed */

    size_t new_cap = (size_t)max_n + 1;
    uint32_t *new_spf = (uint32_t *)calloc(new_cap, sizeof(uint32_t));
    if (!new_spf) return;                       /* out of memory */

    for (size_t i = 2; i < new_cap; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = (uint32_t)i;
            if (i * i < new_cap) {
                for (size_t j = i * i; j < new_cap; j += i) {
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_cap = new_cap;
}

/* Count distinct prime factors of n using the spf array when possible. */
static inline size_t count_distinct_prime_factors(idx_t n) {
    if (n <= 1) return 0;
    size_t count = 0;

    if ((size_t)n < spf_cap) {
        while (n > 1) {
            uint32_t p = spf[n];
            ++count;
            while (n % p == 0) n /= p;
        }
    } else {
        idx_t temp = n;
        for (idx_t p = 2; (idx_t)p * (idx_t)p <= temp; ++p) {
            if (temp % p == 0) {
                ++count;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) ++count;
    }

    return count;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return 0;

    ensure_spf(n);                     /* prepare spf for factorization */
    size_t w = count_distinct_prime_factors(n);   /* ω(n) */
    seq_t b = ((seq_t)1) << (w - 1);   /* b(n) = 2^(ω-1) */

    if ((n & 1) == 0) {                /* even n */
        idx_t n2 = n >> 1;             /* n/2 */
        seq_t b2 = 0;
        if (n2 > 1) {
            size_t w2 = count_distinct_prime_factors(n2);
            b2 = ((seq_t)1) << (w2 - 1);
        }
        return b + b2;
    } else {                           /* odd n */
        return b;
    }
}
