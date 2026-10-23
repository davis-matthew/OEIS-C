/*
 * A108501  Number of factorizations of 4*n into even numbers.
 * https://oeis.org/A108501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of 4*n into even
 *            integers (each factor ≥ 2). The computation is performed by
 *            recursively enumerating factorizations in non‑decreasing order
 *            of factors, which avoids duplicate counts
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_even_factorizations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_even_factorizations.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    seq_t N = (seq_t)n * 4;          /* 4*n, computed in seq_t to avoid overflow */
    return count_even_factorizations((idx_t)N, 2);   /* start with minimal even factor 2 */
}
