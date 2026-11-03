/*
 * A105118  a(n) = prime(n) + prime(n+k) mod 3, where k = (1/2)*(prime(n+1) -
 *          prime(n)).
 * https://oeis.org/A105118
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + prime(n + k) mod 3, where k = (prime(n+1) –
 *            prime(n)) / 2 (integer division, offset 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9853 (2026-11-03)
 * Helpers:   prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A105118: (prime(n) + prime(n+k)) mod 3 with k = (prime(n+1) - prime(n))/2,
 * from the shared growing prime table (the original's table stopped growing
 * at 256 entries and wrote past them). */

#include <stdint.h>
#include "../utils/prime_nth.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t p = prime_nth((uint64_t)n), k = (prime_nth((uint64_t)n + 1) - p) / 2;
    return (seq_t)((p + prime_nth((uint64_t)n + k)) % 3);
}
