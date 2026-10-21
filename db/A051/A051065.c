/*
 * A051065  a(n) = A004128(n) mod 2.
 * https://oeis.org/A051065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + a(floor(n/3))) mod 2 Initial value: a(0) = 0 The
 *            implementation uses direct recursion; since the recurrence only
 *            refers to floor(n/3) the recursion depth is at most log₃(n),
 *            which is negligible for all practical n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t and idx_t */

seq_t a(idx_t n) {
    if (n == 0)   /* base case */
        return 0;
    /* Recurrence: a(n) = (n + a(floor(n/3))) mod 2.
       The addition casts n to seq_t and the result is masked with 1. */
    return (((seq_t)n) + a(n / 3)) & 1;
}
