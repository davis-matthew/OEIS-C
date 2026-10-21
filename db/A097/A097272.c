/*
 * A097272  Least integer with same "mod 2 prime signature" as n.
 * https://oeis.org/A097272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer with the same “mod 2 prime signature” as n
 * Range:     u8 n <= 255, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   _pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_pow_uint.h"

seq_t a(idx_t n)
{
    /* factor n (the index) only using trial division – n ≤ 10 000 in tests */
    uint64_t m = (uint64_t)n;
    uint64_t e0 = 0;
    uint64_t odd_exp[64];
    size_t odd_cnt = 0;

    /* exponent of 2 */
    while ((m & 1) == 0) {
        m >>= 1;
        ++e0;
    }

    /* factor remaining odd part */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            odd_exp[odd_cnt++] = e;
        }
    }
    if (m > 1)          /* m itself is prime */
        odd_exp[odd_cnt++] = 1;

    /* sort odd exponents descending */
    for (size_t i = 1; i < odd_cnt; ++i) {
        uint64_t key = odd_exp[i];
        size_t j = i;
        while (j > 0 && odd_exp[j - 1] < key) {
            odd_exp[j] = odd_exp[j - 1];
            --j;
        }
        odd_exp[j] = key;
    }

    /* assign the smallest odd primes (3,5,7,11,…) */
    static const uint64_t ODD_PRIMES[] = {
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
        37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
        79, 83, 89, 97, 101, 103
    };
    const size_t ODD_PRIMES_COUNT = sizeof(ODD_PRIMES) / sizeof(ODD_PRIMES[0]);

    seq_t result = 1;

    /* multiply by 2^e0 */
    result *= _pow_uint(2, e0);

    /* multiply by odd primes raised to the sorted exponents */
    for (size_t i = 0; i < odd_cnt; ++i) {
        if (i >= ODD_PRIMES_COUNT) break;     /* safety – should not occur for n ≤ 10 000 */
        result *= _pow_uint(ODD_PRIMES[i], odd_exp[i]);
    }

    return result;
}
