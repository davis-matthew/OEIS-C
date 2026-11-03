/*
 * A028432  Golc sequence in base 2. Left to right concatenation of
 *          n,int(log_2(n)),int(log_2(int(log_2(n)))),... in base 2.
 * https://oeis.org/A028432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = left‑to‑right concatenation of n and successive *
 *            floor(log₂) values, written in base 2. For n == 1 an extra
 *            trailing * zero bit is appended
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   ilog2, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"

/* Compute A028432(n) for all n ≥ 1.
 * Uses seq_t (arbitrary unsigned integer type) and idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* Special case n == 1: “1” followed by a final 0 bit. */
    if (n == 1u) {
        /* Start with binary “1” (value 1). */
        return (seq_t)1u * (seq_t)2u;   /* append a 0 bit: 1 << 1 */
    }

    /* Start with binary representation of n itself. */
    seq_t res = (seq_t)n;
    idx_t prev = n;

    /* Append floor(log₂(prev)) while prev > 1. */
    while (prev > 1u) {
        idx_t next = ilog2(prev);          /* floor(log₂(prev)) */
        unsigned len = ilog2(next) + 1u;    /* bit length of next   */
        seq_t pow = pow2(len);                         /* 2^{len} */
        res = res * pow + (seq_t)next;                 /* res << len | next   */
        prev = next;
    }

    return res;
}
