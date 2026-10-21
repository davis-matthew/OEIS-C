/*
 * A055166  Number of Mersenne prime exponents between 10^n and 10^(n+1).
 * https://oeis.org/A055166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = number of Mersenne prime exponents p with 10^n ≤ p <
 *            10^(n+1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t exponents[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL, 21701ULL,
    23209ULL, 44429ULL, 86243ULL, 110503ULL, 132049ULL, 216091ULL, 756839ULL,
    859433ULL, 1257787ULL, 1398269ULL, 2976221ULL, 3021377ULL, 6972593ULL,
    13466917ULL, 13982589ULL, 14950327ULL, 17808997ULL, 27442187ULL,
    30909541ULL, 33369827ULL, 38286973ULL, 40910969ULL, 45968941ULL,
    51159727ULL, 58039157ULL, 63789179ULL, 73068643ULL, 82589933ULL
};

static const uint64_t pow10[] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL
};

seq_t a(idx_t n) {
    /* For n ≥ 8 all known Mersenne prime exponents are < 10^8, */
    /* so the count is zero. */
    if (n >= 8)
        return 0;

    uint64_t low  = pow10[n];
    uint64_t high = pow10[n + 1];
    uint64_t count = 0;

    for (size_t i = 0; i < sizeof(exponents) / sizeof(exponents[0]); ++i) {
        uint64_t e = exponents[i];
        if (e >= low && e < high)
            ++count;
    }

    return (seq_t)count;
}
