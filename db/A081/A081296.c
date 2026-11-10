/*
 * A081296  Primes of the form 2^k - k.
 * https://oeis.org/A081296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k(n) - k(n), where k(n) = A048744(n) is the n-th k with
 *            2^k - k prime (k taken from a table of known values; A000325(k)
 *            = 2^k - k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A081296_A000325_a(idx_t n)
{
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        p += p;
    return p - (seq_t)n;
}

seq_t a(idx_t n)
{
    static const idx_t ks[] = {2, 3, 9, 13, 19, 21, 55, 261, 3415, 4185,
                               7353, 12213, 60244, 83975};
    if (n < 1 || n > sizeof(ks) / sizeof(ks[0]))
        return 0;
    return A081296_A000325_a(ks[n - 1]);
}
