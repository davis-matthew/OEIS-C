/*
 * A068696  Number of primes that are a concatenation nk of n and k for values
 *          of k from 1 to n.
 * https://oeis.org/A068696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime count for concatenated numbers nk, where n is fixed and k ranges from 1 to n */
#include <stdbool.h>
#include "../utils/isPrime.h"

/* powers of 10 up to 10^19 (enough for 64‑bit concatenations) */
static const idx_t POW10[20] = {
    1ULL,
    10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* decimal digit count of a positive integer */
static inline uint8_t dec_digits(idx_t x)
{
    uint8_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* concatenate n and k: n * 10^digits(k) + k */
static inline idx_t concat(idx_t n, idx_t k)
{
    uint8_t dk = dec_digits(k);
    return n * POW10[dk] + k;
}

/* a(n): number of primes among concat(n,k) for k = 1..n */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (isPrime(concat(n, k))) {
            ++count;
        }
    }
    return count;
}
