/*
 * A137439  Sequence is concatenation of S_1, S_2, S_3, ..., where S_k = [45k,
 *          45k-1, 45k-2, ..., 45k-9k].
 * https://oeis.org/A137439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 45*k - (n - prev(k-1) - 1), where * k = smallest integer
 *            with k*(9k+11)/2 >= n, * prev(k) = k*(9k+11)/2 (cumulative
 *            length after block k). * This computes sequence A137439
 *            (concatenation of S_k)
 * Range:     u8 n <= 140, u16 n <= 9922, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   cum_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cum_k.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, not used in OEIS entries */
        return 0;

    /* Find k by doubling and then binary search */
    idx_t low = 1, high = 1;

    while (cum_k(high) < n) {
        /* Avoid endless shift if high would overflow */
        if (high > ((idx_t)-1) >> 1) {
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    low = high >> 1;

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cum_k(mid) >= n)
            high = mid;
        else
            low = mid;
    }

    idx_t k = high;                 /* block index (1‑based) */
    idx_t prev = (idx_t)cum_k(k - 1);/* number of terms before block k    */
    idx_t j = n - prev - 1;         /* zero‑based position inside block k */

    seq_t val = (seq_t)45 * k - (seq_t)j;
    return val;
}
