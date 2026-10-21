/*
 * A060779  a(n) = lcm(tau(n+1), tau(n)), where tau = A000005.
 * https://oeis.org/A060779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(tau(n), tau(n+1))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Small Euclidean GCD for seq_t */
static seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* a(n) = lcm(tau(n), tau(n+1)) */
/* Works for any n >= 1; behaviour for n == 0 is undefined (returns 0) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined offset, but harmless */
    seq_t tau_n   = divisor_count_u64(n);
    seq_t tau_np1 = divisor_count_u64(n + 1);
    seq_t g = _gcd_seq(tau_n, tau_np1);
    return (tau_n / g) * tau_np1;
}
