/*
 * A230654  Numbers n such that tau(n+1) - tau(n) = 4, where tau(n) = the
 *          number of divisors of n (A000005).
 * https://oeis.org/A230654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A230654 – Numbers n such that τ(n+1) – τ(n) = 4 */
/* The function performs a linear search: it scans successive integers m,
   counting those for which the difference in divisor counts equals 4,
   until the nth such integer is found.  This method works for any
   seq_t type (fixed‑size or arbitrary precision). */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* out of range, but not used */
    idx_t found = 0;
    uint64_t m = 1;

    while (found < n) {
        uint64_t tau_m  = divisor_count_u64(m);
        uint64_t tau_m1 = divisor_count_u64(m + 1);
        int diff = (int)(tau_m1 - tau_m);

        if (diff == 4) {
            ++found;
            if (found == n) return (seq_t)m;
        }
        ++m;
    }
    /* Control should never reach here */
    return 0;
}
