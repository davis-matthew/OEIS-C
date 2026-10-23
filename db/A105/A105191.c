/*
 * A105191  Number of divisors of n100000000001.
 * https://oeis.org/A105191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of (n * 10^12 + 100000000001)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   count_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_divisors.h"  /* assumed available utility */

seq_t a(idx_t n)
{
    /* 10^12 and 10^11 + 1 */
    const unsigned long long T = 1000000000000ULL;
    const unsigned long long S = 100000000001ULL;

    /* compute n*T + S with 128‑bit arithmetic to avoid overflow for large n */
    unsigned __int128 prod = (unsigned __int128)n * T;
    unsigned long long N = (unsigned long long)(prod + S);

    /* use the bundled divisor count routine */
    return count_divisors(N);
}
