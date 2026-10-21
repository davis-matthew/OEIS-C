/*
 * A201222  Number of ways to place k non-attacking knights on a 2 X n
 *          horizontal cylinder, summed over all k>=0.
 * https://oeis.org/A201222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas( 2n / gcd(4,n) ) ^ gcd(4,n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   _gcd, _lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd.h"
#include "../utils/_lucas.h"

/* OEIS A201222: number of independent sets on a 2×n
   horizontal cylinder (sum over k ≥ 0 non‑attacking knights) */
seq_t a(idx_t n)
{
    /* Number of connected cycles (components) */
    idx_t g = _gcd((idx_t)4, n);

    /* Cycle length = 2n / g, fits into idx_t for all n tested */
    idx_t k = ((idx_t)2 * n) / g;

    seq_t luc = _lucas(k);

    /* Result = luc ^ g (g ≤ 4) */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < g; ++i) {
        result *= luc;
    }
    return result;
}
