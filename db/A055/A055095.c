/*
 * A055095  a(n) = 2*A000120(A003188(A055094(n))) - (n-1) =
 *          2*A005811(A055094(n)) - (n-1).
 * https://oeis.org/A055095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A005811(A055094(n)) - (n-1); the weight is computed
 *            directly from the quadratic-residue bit string of length n-1 as
 *            b_1 + #{i>=2 : b_i != b_{i-1}} (number of 1-bits in its Gray
 *            code)
 * Range:     i8 n <= 239, i16 n <= 4096, i32 n <= 4096, i64 n <= 4096,
 *            i128 n <= 4096, big any n
 * Validated: standard, pass, 121 terms through n = 3944 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    size_t m = (size_t)n;
    bool *is_res = (bool *)calloc(m, sizeof(bool));
    if (!is_res)
        return 0;

    /* mark squares mod n incrementally: (x+1)^2 = x^2 + 2x + 1 */
    uint64_t sq = 0;
    for (uint64_t x = 0; x < (uint64_t)n; ++x) {
        is_res[(size_t)sq] = true;
        sq = (uint64_t)(((unsigned __int128)sq + 2 * (unsigned __int128)x + 1) % (unsigned __int128)n);
    }

    /* bits b_1..b_{n-1}; Gray-code weight = b_1 + number of changes */
    uint64_t wt = is_res[1] ? 1 : 0;
    for (idx_t b = 2; b < n; ++b) {
        if (is_res[(size_t)b] != is_res[(size_t)(b - 1)])
            ++wt;
    }
    free(is_res);

    return (seq_t)2 * (seq_t)wt - ((seq_t)n - 1);
}
