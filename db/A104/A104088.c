/*
 * A104088  Largest prime <= 3^n.
 * https://oeis.org/A104088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime ≤ 3^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_idx, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t x = ipow_idx((seq_t)3, n);
    while (!is_prime_seq(x)) {
        x -= 1;
    }
    return x;
}
