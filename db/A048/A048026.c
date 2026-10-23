/*
 * A048026  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t>=1, 5t<=T, t<=L} C(T,5t) * C(L,t), where T =
 *            floor((n-2)/3) and L = n - T
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle small n where (n-2) would underflow */
    if (n < 2)
        return 0;

    /* Compute T and L */
    idx_t T = (n - 2) / 3;          /* small elements count */
    idx_t L = n - T;               /* large elements count */

    /* If no subset can satisfy the condition, return 0 */
    if (T == 0 || L == 0)
        return 0;

    /* Max t such that 5t <= T and t <= L */
    idx_t tmax = T / 5;
    if (tmax > L)
        tmax = L;
    if (tmax == 0)
        return 0;

    seq_t total = 0;
    for (idx_t t = 1; t <= tmax; ++t) {
        seq_t small_part = binom(T, 5 * t);   /* C(T,5t) */
        seq_t large_part = binom(L, t);       /* C(L,t) */
        seq_t prod = small_part * large_part; /* C(T,5t) * C(L,t) */
        total += prod;
    }
    return total;
}
