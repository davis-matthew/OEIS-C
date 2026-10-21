/*
 * A069624  Number of different values taken by the integer part of n^(1/k)
 *          for all k > 1.
 * https://oeis.org/A069624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct integer parts floor(n^(1/k)) for all k
 *            > 1
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"

static seq_t kth_root_floor(seq_t n, idx_t k)
{
    /* Binary search for the largest integer r with r^k <= n.
       n > 0 and k > 1. */
    seq_t low = 1;
    seq_t high = low;

    /* Find an upper bound `high` such that high^k > n.
       Since n <= 10000 for the intended test range we use a
       simple doubling loop.  For larger values this can be
       replaced by a more sophisticated bound using bit lengths. */
    while (true) {
        seq_t hpow = big_pow(high, (seq_t)k);
        if (hpow > n)
            break;
        low = high;
        high <<= 1;                /* high = high * 2 */
        /* Handle possible overflow of high (unlikely for the small
           values considered in the tests).  If overflow happened, we
           stop and use the maximum representable value. */
        if (high == 0) {
            high = ~ (seq_t)0;
            break;
        }
    }

    /* Binary search between low and high */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t mpow = big_pow(mid, (seq_t)k);
        if (mpow <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. For n == 0 we return 0,
       which is irrelevant for the OEIS offset. */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;
    seq_t count = 0;
    seq_t prev = 0;

    for (idx_t k = 2; ; ++k) {
        seq_t val = kth_root_floor(N, k);
        if (val != prev) {
            ++count;
            prev = val;
        }
        if (val == 1)
            break;            /* all larger k give value 1 */
    }
    return count;
}
