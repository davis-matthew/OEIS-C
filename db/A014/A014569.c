/*
 * A014569  Super-3 Numbers (3n^3 contains substring '333' in its decimal
 *          expansion).
 * https://oeis.org/A014569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   has_333
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computed by scanning for integers n such that 3*n^3 contains the
 * decimal substring "333".
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_333.h"

/* a(n) returns the nth term of the Super‑3 Numbers sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        /* Compute 3*m^3 using seq_t arithmetic. */
        seq_t val = (seq_t)3;
        val *= (seq_t)m;
        val *= (seq_t)m;
        val *= (seq_t)m;
        if (has_333(val)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
