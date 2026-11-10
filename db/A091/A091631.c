/*
 * A091631  Next prime associated with A091628.
 * https://oeis.org/A091631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007918(A091628(n)+1), where A091628(n) = 10*A091628(n-1)
 *            - 7, A091628(1) = 23; i.e. the least prime > A091628(n), found
 *            by searching upward
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t x = 23;
    for (idx_t i = 1; i < n; ++i)
        x = x * 10 - 7;
    seq_t c = x + 1;
    while (!is_prime_seq(c)) c++;
    return c;
}
