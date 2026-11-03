/*
 * A202035  Greatest prime residue of x^n (mod n) for x=0..n-1, or 0 if no
 *          such prime exists.
 * https://oeis.org/A202035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = greatest prime residue k^n (mod n) for 0 ≤ k < n, or 0 if none. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* no definition for n < 1 */
    seq_t best = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t r = powmod((seq_t)k, (uint64_t)n, (seq_t)n);
        if (r > best && is_prime_seq(r))
            best = r;
    }
    return best;
}
