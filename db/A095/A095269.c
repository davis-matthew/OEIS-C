/*
 * A095269  Diving index of 4n+3.
 * https://oeis.org/A095269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the diving index of 4n+3 by brute‑force search:
   a(n) = the smallest u > 1 with Σ_{i=1..u} J(i/(4n+3)) = -1.
   If no such u exists, returns 0. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)4 * n + 3;   /* odd number 4n+3 */
    int sum = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        int j = jacobi_symbol(i, m);   /* -1, 0, or 1 */
        sum += j;
        if (sum == -1 && i > 1)
            return (seq_t)i;
    }
    return (seq_t)0;
}
