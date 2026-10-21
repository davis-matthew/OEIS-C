/*
 * A091484  Recamán's Fibonacci variation : a(1)=a(2)=1 then a(n) =
 *          a(n-1)+a(n-2)-F(n) if that number is >0 and not already in the
 *          sequence; a(n) = a(n-1)+a(n-2)+F(n) otherwise where F(n) denotes
 *          the n-th Fibonacci number.
 * https://oeis.org/A091484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán's Fibonacci variation (OEIS A091484).
   Linear recurrence: a(n+8) = 7*a(n+4) - a(n) for n ≥ 3.
   Initial values: a(1)=1, a(2)=1, a(3)=4, a(4)=2, a(5)=11,
                    a(6)=5, a(7)=3, a(8)=29, a(9)=66, a(10)=40.
   The function computes a(n) for any n ≥ 1 in O(n) time using an
   8‑element circular buffer. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values; index 0 unused */
    static const seq_t init[11] = {
        0ULL,
        1ULL, 1ULL, 4ULL, 2ULL, 11ULL, 5ULL, 3ULL, 29ULL, 66ULL, 40ULL
    };

    if (n <= 10) {
        return init[(size_t)n];
    }

    /* ring buffer holds the last 8 computed terms */
    seq_t ring[8];
    for (idx_t i = 1; i <= 10; ++i) {
        ring[i & 7] = init[(size_t)i];
    }

    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = 7 * ring[(i - 4) & 7] - ring[(i - 8) & 7];
        ring[i & 7] = val;
    }

    return ring[n & 7];
}
