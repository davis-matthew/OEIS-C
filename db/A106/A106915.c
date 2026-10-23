/*
 * A106915  Primes of the form 3x^2 + 2xy + 5y^2, with x and y any integer.
 * https://oeis.org/A106915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation by the quadratic form 3x^2 + 2xy + 5y^2.
   The function searches primes and checks whether they can be expressed as
   3x^2 + 2xy + 5y^2.  For primes the necessary condition
   (-56 | p) = 1 reduces to p ≡ 3,5,13,19,27,45 (mod 56), so we skip
   most candidates quickly.  For each prime that satisfies the residue
   test we search for integer solutions (x, y) by solving the quadratic
   equation in x for every |y| ≤ √p.  The first n such primes are returned
   in OEIS order. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/integer_sqrt.h"

#define ALLOWED_MODS_COUNT 6
static const int allowed_mods[ALLOWED_MODS_COUNT] = {3, 5, 13, 19, 27, 45};

/* Check whether an odd prime p can be represented
   by 3x^2 + 2xy + 5y^2.  */
static bool represents_prime(uint64_t p)
{
    /* Warn: use 128‑bit arithmetic to avoid overflow. */
    uint64_t ymax = (uint64_t)integer_sqrt((idx_t)p);
    for (int64_t y = -(int64_t)ymax; y <= (int64_t)ymax; ++y)
    {
        uint64_t uy = (y >= 0) ? (uint64_t)y : (uint64_t)(-y);
        unsigned __int128 y2 = (unsigned __int128)uy * (unsigned __int128)uy;

        /* Discriminant: D = 12p – 56y^2 */
        unsigned __int128 Dbig = (unsigned __int128)12 * p - (unsigned __int128)56 * y2;
        if (Dbig > (unsigned __int128)UINT64_MAX)  /* too large for our sqrt routine */
            continue;
        uint64_t D = (uint64_t)Dbig;
        if (D == 0)
            continue;

        uint64_t sqrtD = integer_sqrt((idx_t)D);
        if ((uint64_t)sqrtD * (uint64_t)sqrtD != D)
            continue;

        /* Solutions for x: x = (-2y ± sqrtD) / 6 */
        int64_t n1 = -2 * y + (int64_t)sqrtD;
        if (n1 % 6 == 0)
            return true;
        int64_t n2 = -2 * y - (int64_t)sqrtD;
        if (n2 % 6 == 0)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset is 1 */

    idx_t count = 0;
    uint64_t candidate = 3;        /* first odd prime */

    while (1) {
        if (is_prime(candidate)) {
            /* Quick residue test: only needed residues mod 56 */
            uint64_t r = candidate % 56;
            bool ok = false;
            for (int i = 0; i < ALLOWED_MODS_COUNT; ++i) {
                if (r == (uint64_t)allowed_mods[i]) {
                    ok = true;
                    break;
                }
            }
            if (ok && represents_prime(candidate)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate += 2;   /* skip even numbers */
    }
}
