/*
 * A166560  Primes of the form 100*n+9.
 * https://oeis.org/A166560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 100·k + 9 (OEIS A166560)
 * Range:     u8 n <= 1, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 929 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is the first term */
    idx_t count = 0;
    idx_t k = 1;

    for (;;) {
        seq_t cand = (seq_t)100 * (seq_t)k + (seq_t)9;
        if (is_prime_seq(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++k;
    }
}
