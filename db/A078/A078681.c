/*
 * A078681  Start with n; iterate the process x -> ceiling(3x/2) until reach a
 *          prime. Sequence gives number of steps to reach a prime or 0 if no
 *          prime is ever reached.
 * https://oeis.org/A078681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> ceil(3x/2) starting from n
 *            until a prime occurs. The steps counter is returned
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   mod_pow_uint128, mul_mod_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow_uint128.h"
#include "../utils/mul_mod_uint128.h"

/* Deterministic Miller–Rabin bases sufficient for all 128‑bit numbers. */
static const unsigned long long MR_BASES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
    19ULL, 23ULL, 29ULL, 31ULL, 37ULL
};
static const size_t MR_BASES_COUNT =
    sizeof(MR_BASES) / sizeof(MR_BASES[0]);

/* Return true if x is a prime number (deterministic Miller–Rabin
 * for 128‑bit integers). */
static bool is_prime_u128(unsigned __int128 x)
{
    if (x < 2)
        return false;
    if (x == 2 || x == 3)
        return true;
    if ((x & 1U) == 0U)
        return false;

    /* factor x-1 as d * 2^s */
    unsigned __int128 d = x - 1;
    unsigned int s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    for (size_t i = 0; i < MR_BASES_COUNT; ++i) {
        unsigned long long a64 = MR_BASES[i];
        if ((unsigned __int128)a64 % x == 0U)
            continue;
        unsigned __int128 a = (unsigned __int128)a64;
        unsigned __int128 y = mod_pow_uint128(a, d, x);
        if (y == 1 || y == x - 1)
            continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r) {
            y = mul_mod_uint128(y, y, x);
            if (y == x - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety guard – not used for OEIS */

    unsigned __int128 x = (unsigned __int128)n;
    seq_t steps = 0;

    /* Perform at least one step even if n is prime. */
    do {
        ++steps;
        unsigned __int128 half = (x + 1U) >> 1U;
        x += half;
    } while (!is_prime_u128(x));

    return steps;
}
