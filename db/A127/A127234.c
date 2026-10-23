/*
 * A127234  a(n) = n! (sum(1..n-1))!.
 * https://oeis.org/A127234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ((n-1)*n/2)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but guard for safety */
    seq_t f1 = factorial(n);
    idx_t sum = n * (n - 1) / 2;      /* integer arithmetic, fits in idx_t for n up to 10000 */
    seq_t f2 = factorial(sum);
    return f1 * f2;
}
