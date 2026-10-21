/*
 * A078489  a(n)=j such that binomial(n,j)<binomial(n-1,j-2).
 * https://oeis.org/A078489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A078489
   Formula: a(n) = smallest j (j ≥ 2) such that
              C(n,j) ≤ C(n-1,j-2)
   Equivalent inequality: n*(n-j+1) ≤ j*(j-1)
   Binary search is used for efficiency. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset starts at 1; for n == 1 the answer is 2. */
    if (n < 2) return (seq_t)2;

    /* Search j in [2, n] for the smallest j satisfying the inequality. */
    idx_t low = 2, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* Use unsigned __int128 to avoid overflow in the products. */
        unsigned __int128 left  = (unsigned __int128)n * (n - mid + 1);
        unsigned __int128 right = (unsigned __int128)mid * (mid - 1);

        if (left <= right)
            high = mid;          /* condition holds, try smaller j */
        else
            low = mid + 1;       /* condition fails, need larger j */
    }
    return (seq_t)low;
}
