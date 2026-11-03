/*
 * A103159  a(n) = GCD(reverse(n), reversed(n+1)).
 * https://oeis.org/A103159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = gcd(reverse(n), reverse(n+1))
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9477 (2026-11-01)
 * Helpers:   gcd, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t rev_n   = reverse_digits((seq_t)n);
    seq_t rev_np1 = reverse_digits((seq_t)(n + 1));
    return gcd(rev_n, rev_np1);
}
