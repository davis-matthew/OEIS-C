/*
 * A025552  LCM of {C(0,0), C(1,0), ..., C(n, floor(n/2))}.
 * https://oeis.org/A025552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,…,n) / b(n), where * b(n) = 1 if n+1 or n+2 is a
 *            power of two, * b(n) = 2 otherwise. * * This implements the LCM
 *            of central binomial coefficients * C(0,0), C(1,0), …, C(n,⌊n/2⌋)
 *            as given by OEIS A025552. * For n < 0 the behaviour is
 *            irrelevant
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Test if an unsigned integer is a power of two (x > 0). */
static inline bool is_power_of_two(idx_t x)
{
    return x != 0ULL && (x & (x - 1ULL)) == 0ULL;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of the lcm(1..k) computed so far. */
    static seq_t lcm_cache = (seq_t)1;
    static idx_t last_n   = 0;

    /* Extend the cached lcm up to n if necessary. */
    if (n > last_n) {
        for (idx_t m = last_n + 1; m <= n; ++m) {
            seq_t val = (seq_t)m;
            seq_t g   = gcd(lcm_cache, val);
            lcm_cache = (lcm_cache / g) * val;   /* lcm = lcm/g * val */
        }
        last_n = n;
    }

    /* Determine the divisor b(n). */
    bool div_by_two = !(is_power_of_two(n + 1) || is_power_of_two(n + 2));
    seq_t b = div_by_two ? (seq_t)2 : (seq_t)1;

    return lcm_cache / b;
}
