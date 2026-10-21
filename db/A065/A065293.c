/*
 * A065293  Number of values of s, 0 <= s <= n-1, such that 2^s mod n = s.
 * https://oeis.org/A065293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ s | 0 <= s < n, 2^s mod n = s }
 * Range:     u8 n <= 603, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: n must be at least 1 as per definition */
    seq_t count = 0;
    seq_t mod_n = (seq_t)n;
    /* 2^0 mod n = 1 mod n */
    seq_t power = 1 % mod_n;

    for (idx_t s = 0; s < n; ++s)
    {
        if (power == (seq_t)s)
            ++count;
        /* Advance to 2^(s+1) mod n */
        power = (power * 2) % mod_n;
    }
    return count;
}
