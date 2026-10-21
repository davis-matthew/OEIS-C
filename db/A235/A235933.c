/*
 * A235933  Numbers coprime to 35.
 * https://oeis.org/A235933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is coprime to 35 (i.e., not
 *            divisible by 5 or 7)
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return how many integers <= x are coprime to 35. */
static inline seq_t count_coprime(seq_t x)
{
    return x - x/5 - x/7 + x/35;
}

/* Offset 1: a(1) is the first term. */
seq_t a(idx_t n)
{
    /* The index n is unsigned 64‐bit; convert once to seq_t for
       arithmetic. */
    const seq_t sn = (seq_t)n;
    /* Binary search for the smallest integer x such that
       count_coprime(x) >= sn.  Use an expanding upper bound to avoid
       overflow in the sequence type. */
    seq_t lo = 1;
    seq_t hi = 1;
    while (count_coprime(hi) < sn)
    {
        /* Handle potential overflow: if hi * 2 wraps to a smaller value,
           saturate to the maximum representable value. */
        seq_t hi_next = hi * 2;
        if (hi_next <= hi)
        {
            hi = ~((seq_t)0);   /* maximum value of seq_t */
            break;
        }
        hi = hi_next;
    }
    /* Standard binary search on [lo, hi]. */
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo) / 2;
        if (count_coprime(mid) >= sn)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}
