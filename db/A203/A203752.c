/*
 * A203752  Vandermonde sequence using x^2 - xy + y^2 applied to
 *          (0,1,1,2,2,...,floor(n/2)).
 * https://oeis.org/A203752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} ( floor(i/2)^2 - floor(i/2)*floor(j/2)
 *            + floor(j/2)^2 )
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n = 0, return 0 as a safe sentinel */
    if (n == 0) return 0;

    /* Highest value of floor(i/2) for 1 ≤ i ≤ n */
    idx_t max_v = n / 2;
    seq_t result = 1;

    /* Contributions from pairs that involve the zero element
       and from identical m-values.  For each v ≥ 1,
       if it occurs twice (freq = 2) we multiply by v⁶ (v⁴ from pairs
       with 0 and v² from the pair of equal values);
       if it occurs once (freq = 1) we multiply by v².
    */
    for (idx_t v = 1; v <= max_v; ++v) {
        seq_t base = (seq_t)v;
        int freq  = (n >= 2*v + 1) ? 2 : 1;   /* 2 if two indices, else 1 */

        if (freq == 2) {           /* v appears twice  → v⁶ */
            for (int k = 0; k < 6; ++k) result *= base;
        } else {                   /* v appears once   → v² */
            for (int k = 0; k < 2; ++k) result *= base;
        }
    }

    /* Cross pairs of distinct m-values (both ≥ 1) */
    for (idx_t i = 1; i <= max_v; ++i) {
        int fi = (n >= 2*i + 1) ? 2 : 1;   /* frequency of i */

        for (idx_t j = i + 1; j <= max_v; ++j) {
            int fj = (n >= 2*j + 1) ? 2 : 1;   /* frequency of j */

            /* Compute the small integer term i² - i·j + j² */
            uint64_t ii   = (uint64_t)i * i;
            uint64_t ij   = (uint64_t)i * j;
            uint64_t jj   = (uint64_t)j * j;
            uint64_t t    = ii - ij + jj;       /* fits in 32 bits */

            int exp = fi * fj;                  /* 1, 2 or 4 */
            seq_t term = (seq_t)t;

            for (int k = 0; k < exp; ++k) result *= term;
        }
    }

    return result;
}
