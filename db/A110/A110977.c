/*
 * A110977  Sequence of denominators associated with the continued fraction
 *          based on the sequence d(n)= distance of n from closest prime (
 *          A051699).
 * https://oeis.org/A110977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = d(n) * a(n-1) + a(n-2) where a(0) = 1, a(1) = 1 and d(n)
 *            is the distance from n to the nearest prime
 * Range:     u8 n <= 20, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t dist_to_prime(idx_t n)
{
    /* distance from n to the closest prime */
    uint64_t un = (uint64_t)n;
    if (is_prime(un))
        return 0;

    uint64_t up   = un + 1;          /* first upward candidate */
    uint64_t down = (un > 0 ? un - 1 : 0); /* first downward candidate */

    for (uint64_t d = 1; ; ++d) {
        bool up_ok   = is_prime(up);
        bool down_ok = false;
        if (down > 0)
            down_ok = is_prime(down);

        if (up_ok || down_ok)
            return (seq_t)d;

        ++up;
        if (down > 0)
            --down;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t a_prev_prev = 1; /* a(0) */
    seq_t a_prev      = 1; /* a(1) */
    seq_t a_curr      = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t d = dist_to_prime(i);
        a_curr = d * a_prev + a_prev_prev;
        a_prev_prev = a_prev;
        a_prev      = a_curr;
    }
    return a_curr;
}
