/*
 * A051584  Number of integer-sided triangles of area n.
 * https://oeis.org/A051584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8991, u16 n <= 8991, u32 n <= 8991, u64 n <= 8991,
 *            u128 n <= 8991, big any n
 * Helpers:   factorize, gen_divisors, isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A051584: Number of integer‑sided triangles of area n.
 * Uses the identity: for positive integers x,y,z,
 *    area² = (x+y+z)·x·y·z .
 * Hence we count solutions of
 *    xyz(x+y+z) = n²
 * with x ≤ y ≤ z to avoid permutations.
 *
 * Complexity: O(d(n²)²), where d(m) is number of divisors of m.
 *   For n ≤ 10⁴ this is far below 2 s.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"
#include "../utils/gen_divisors.h"
#include "../utils/isqrt128.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n<1 */

    uint64_t n2 = (uint64_t)n * (uint64_t)n;

    /* factorization of n² */
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = factorize((uint64_t)n, primes, exps);

    /* generate all divisors of n² */
    uint64_t divs[2000];
    size_t dcnt = 0;
    gen_divisors(n2, primes, exps, pcnt, 1, divs, &dcnt);

    /* sort divisors to process in non‑decreasing order */
    for (size_t i = 0; i < dcnt - 1; i++)
        for (size_t j = i + 1; j < dcnt; j++)
            if (divs[i] > divs[j]) {
                uint64_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    seq_t count = 0;
    for (size_t i = 0; i < dcnt; i++) {
        uint64_t x = divs[i];
        for (size_t j = i; j < dcnt; j++) {
            uint64_t y = divs[j];

            __int128 prodxy = (__int128)x * (__int128)y;
            if (prodxy > n2) continue;
            if (n2 % (uint64_t)prodxy != 0) continue;

            uint64_t t = n2 / (uint64_t)prodxy;          /* t = z(x+y+z) */

            uint64_t sumxy = x + y;
            __int128 D = (__int128)sumxy * sumxy + (__int128)4 * t;

            uint64_t sqrtD = isqrt128(D);
            if ((__int128)sqrtD * sqrtD != D) continue;

            if (sqrtD <= sumxy) continue;              /* z would be <= 0 */

            uint64_t z = (sqrtD - sumxy) / 2;
            if (z == 0) continue;
            if (z < y) continue;                       /* keep ordering x≤y≤z */

            /* Verify the equation to guard against any overflow bug */
            __int128 lhs = (__int128)x * y * z * (sumxy + z);
            if (lhs == n2) count++;
        }
    }

    return count;
}
