/*
 * A023100  a(n) is least k such that k and 8k are anagrams in base n (written
 *          in base 10).
 * https://oeis.org/A023100
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that k and 8k are anagrams in base n. *
 *            Computation: brute force search over multiples of
 *            (n-1)/gcd(n-1,7)
 * Range:     u8 none, u16 n <= 9, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_anagram
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_anagram.h"

/* Euclidean gcd for 64‑bit unsigned integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 9)
        return 0; /* undefined for n < 9, arbitrary return */

    uint64_t base = (uint64_t)n;
    uint64_t baseMinus1 = base - 1;
    uint64_t g = gcd_uint64(baseMinus1, 7ULL);
    uint64_t step_val = baseMinus1 / g;

    seq_t step = (seq_t)step_val;
    seq_t k = step; /* first candidate */

    while (true) {
        if (is_anagram(k, (unsigned)base))
            return k;
        k += step;
    }
}
