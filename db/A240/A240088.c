/*
 * A240088  The number of ways of writing n as an ordered sum of a triangular
 *          number (A000217), a square (A000290) and a pentagonal number
 *          (A000326).
 * https://oeis.org/A240088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (t,s,p) | t triangular, s square, p pentagonal, t + s
 *            + p = n }|
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute pentagonal numbers up to n */
    bool *pent = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!pent)
        return (seq_t)0;  /* out of memory: return 0 as a safe fallback */

    /* Initialize all to false */
    for (idx_t i = 0; i <= n; ++i)
        pent[i] = false;

    for (idx_t y = 0; ; ++y) {
        seq_t p = (seq_t)y * (3 * y - 1) / 2;
        if (p > n)
            break;
        pent[(size_t)p] = true;
    }

    /* Main counting loop */
    seq_t count = 0;

    for (idx_t r = 0; ; ++r) {
        seq_t t = (seq_t)r * (r + 1) / 2;
        if (t > n)
            break;
        /* For each triangular t, iterate over squares s */
        for (idx_t s = 0; ; ++s) {
            seq_t sq = (seq_t)s * s;
            if (sq > n - t)
                break;
            seq_t p = n - t - sq;
            if (p <= n && pent[(size_t)p])
                ++count;
        }
    }

    free(pent);
    return count;
}
