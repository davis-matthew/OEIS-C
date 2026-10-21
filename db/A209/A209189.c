/*
 * A209189  Smallest prime factor of n^2 + n + 1.
 * https://oeis.org/A209189
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest prime factor of n^2 + n + 1
 */
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. For n < 2, result is undefined. */
    if (n < 2)
        return 0;

    /* Compute n^2 + n + 1 in 128‑bit to avoid overflow for our test range. */
    uint64_t un = (uint64_t)n;
    unsigned __int128 v128 = (unsigned __int128)un * un + un + 1;
    uint64_t v = (uint64_t)v128;   /* safe for n <= ~10^9 (our test range is <=10^4) */

    uint64_t spf = _smallest_prime_factor(v);
    return (seq_t)spf;
}
