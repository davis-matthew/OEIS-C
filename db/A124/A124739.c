/*
 * A124739  a(n) = sum of those positive integers which are coprime to both n
 *          and n+1 and which are <= n.
 * https://oeis.org/A124739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of k (1 ≤ k ≤ n) with gcd(k, n) = 1 and gcd(k, n+1) =
 *            1
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, k is coprime to n*(n+1)) */

#include <stdint.h>

static inline void add_prime(uint64_t primes[], int *pcnt, uint64_t p)
{
    for (int i = 0; i < *pcnt; ++i) {
        if (primes[i] == p) return;
    }
    primes[(*pcnt)++] = p;
}

static inline void factor_and_add(uint64_t m, uint64_t primes[], int *pcnt)
{
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            add_prime(primes, pcnt, p);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) add_prime(primes, pcnt, m);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n=0, not needed for OEIS offset */

    /* collect distinct primes of n and n+1 */
    uint64_t primes[64];
    int pcnt = 0;
    factor_and_add((uint64_t)n, primes, &pcnt);
    factor_and_add((uint64_t)n + 1, primes, &pcnt);

    /* total sum of 1..n in seq_t */
    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;

    /* inclusion–exclusion over subsets of primes */
    unsigned subsets = 1U << pcnt;
    for (unsigned mask = 1; mask < subsets; ++mask) {
        unsigned __int128 prod = 1;
        int bits = 0;
        for (int i = 0; i < pcnt; ++i) {
            if (mask & (1U << i)) {
                ++bits;
                prod *= primes[i];
                if (prod > n) break;           /* product already too large */
            }
        }
        if (prod > n) continue;                /* no multiples ≤ n */

        uint64_t prod64 = (uint64_t)prod;
        uint64_t cnt = n / prod64;
        unsigned __int128 tmp = (unsigned __int128)prod64 * cnt * (cnt + 1) / 2;
        seq_t sumProd = (seq_t)tmp;

        if (bits & 1)   /* odd number of primes → subtract */
            total -= sumProd;
        else            /* even number of primes → add */
            total += sumProd;
    }

    return total;
}
