/*
 * A200787  Number of 0..n arrays x(0..4) of 5 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((1 + n)^2 * (12 + 44*n + 33*n^2 + 7*n^3)) / 12
 *            Equivalent integer form: a(n) = (7*n^5 + 47*n^4 + 117*n^3 +
 *            133*n^2 + 68*n + 12) / 12 (valid for all n ≥ 1, offset 1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 92, u64 n <= 139, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 128‑bit intermediate to avoid overflow for n up to ~10⁴ */
    unsigned __int128 u  = (unsigned __int128)n;
    unsigned __int128 u2 = u * u;
    unsigned __int128 u3 = u2 * u;
    unsigned __int128 u4 = u3 * u;
    unsigned __int128 u5 = u4 * u;

    unsigned __int128 num = 7 * u5
                          + 47 * u4
                          + 117 * u3
                          + 133 * u2
                          + 68 * u
                          + 12;

    return (seq_t)(num / 12);
}
