/*
 * A178480  For n=0,1,2,... list all products of the first n primes raised to
 *          some positive power not exceeding n.
 * https://oeis.org/A178480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th term in the concatenated list of * products of
 *            the first m primes raised to exponents 1..m for each m, *
 *            enumerated with the last prime exponent varying slowest
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow, pow_m_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/pow_m_m.h"

/* first few primes – enough for all n that fit in a 64‑bit index */
static const uint32_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n≥1, but avoid UB */

    unsigned __int128 cumulative = 0;           /* sum of blocks already passed   */
    for (unsigned int m = 0; ; ++m) {
        /* the remaining amount needed to reach n */
        unsigned __int128 remaining = (unsigned __int128)n - cumulative;
        unsigned __int128 block = pow_m_m(m, remaining);

        if (cumulative + block >= n) {        /* the desired block is found */
            idx_t local = (idx_t)((unsigned __int128)n - cumulative - 1);
            if (m == 0) return (seq_t)1;      /* block 0 contains only the empty product */

            seq_t result = 1;
            unsigned __int128 idx = local;
            unsigned int base = m;           /* radix for the mixed radix representation */

            for (unsigned int pos = 0; pos < m; ++pos) {
                unsigned int digit = (unsigned int)(idx % base);   /* 0 .. m-1 */
                idx /= base;
                seq_t power = big_pow((seq_t)PRIMES[pos], (seq_t)(digit + 1));
                result *= power;
            }
            return result;
        }
        cumulative += block;                  /* proceed to the next block */
    }
}
