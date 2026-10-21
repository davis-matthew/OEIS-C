/*
 * A225765  Least k>0 such that k^3+n is prime, or 0 if there is no such k.
 * https://oeis.org/A225765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that k³+n is prime, or 0 if n is a perfect
 *            cube >1
 * Range:     u8 n <= 30, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   _int_cube_root_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implemented by searching for k from 1 upwards and using the provided prime‑test. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/_int_cube_root_u64.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Return 0 for perfect cubes >1 (case n = m³, m>1). */
    uint64_t m = _int_cube_root_u64((uint64_t)n);
    if (m > 1 && (__int128)m * m * m == n)
        return (seq_t)0;

    /* Search for the smallest k such that k³ + n is prime. */
    uint64_t k = 1;
    for (;; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t val = kk * kk * kk + (seq_t)n;
        if (is_prime_big(val))
            return (seq_t)k;
    }
}
