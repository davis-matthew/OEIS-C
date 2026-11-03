/*
 * A120486  Partial sums of A000188.
 * https://oeis.org/A120486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} phi(k) * floor(n / k^2)
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8328 (2026-11-01)
 * Helpers:   isqrt, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the pre‑implemented totient function phi(k) and the helper
 * _floor_isqrt_u64() to obtain floor(sqrt(n)). */

#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* For n == 0 (outside the offset) the sequence is undefined,
     * but returning 0 does not affect correctness for n >= 1. */
    seq_t result = 0;

    if (n < 1) {
        return result;
    }

    idx_t limit = isqrt(n);
    for (idx_t k = 1; k <= limit; ++k) {
        /* k^2 fits into idx_t because k <= sqrt(n). */
        idx_t k2   = k * k;
        idx_t term = n / k2;            /* floor(n / k^2) */
        seq_t phi_k = totient(k);           /* φ(k) */
        result += phi_k * (seq_t)term;  /* accumulate in seq_t */
    }
    return result;
}
