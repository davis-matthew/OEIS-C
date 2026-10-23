/*
 * A166071  Sum of the atomic numbers in the n-th column of the Janet table of
 *          the first 120 elements.
 * https://oeis.org/A166071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   a(n) = sum of atomic numbers in the n‑th column of the Janet
 *            table of the first 120 elements (n = 1 … 32)
 * Range:     u8 n <= 20, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   get_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_col.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t sums[33] = {0};

    if (!init) {
        for (idx_t i = 1; i <= 120; ++i) {
            idx_t col = get_col(i);
            if (col >= 1 && col <= 32) {
                sums[(size_t)col] += (seq_t)i;
            }
        }
        init = true;
    }
    if (n >= 1 && n <= 32) {
        return sums[(size_t)n];
    }
    return (seq_t)0;
}
