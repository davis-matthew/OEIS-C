/*
 * A225231  Schur numbers S(3,n).
 * https://oeis.org/A225231
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n ≥ 5, a(n) = (3*n*n - 7*n)/2 + c, where c = 3 if n mod 4 is
 *            0 or 1, else c = 4 Special values: a(3) = 9, a(4) = 16. For n <
 *            3 the result is undefined (returns 0)
 * Range:     u8 n <= 10, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, may be unused but harmless */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)16;
    if (n < 3)  return (seq_t)0;   /* undefined for n < 3 */

    /* Compute in seq_t to avoid overflow for all supported sizes */
    seq_t N   = (seq_t)n;
    seq_t num = (seq_t)3 * N * N - (seq_t)7 * N;  /* 3n^2 - 7n, always even for n ≥ 5 */
    seq_t val = num / (seq_t)2;

    /* Determine the constant term c based on n mod 4 */
    seq_t c = ((n & 3) <= 1) ? (seq_t)3 : (seq_t)4;

    return val + c;
}
