/*
 * A216485  a(n) is the least value of k such that k*n uses only the digit 2,
 *          or a(n) = -1 if no such multiple exists.
 * https://oeis.org/A216485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = least k such that k*n = 222…2 (a repdigit of 2’s), or -1
 *            if no such multiple exists. For n ≥ 1. The algorithm (1) skips
 *            the trivial non‑existence cases n≡0 (mod 4) or n≡0 (mod 5), (2)
 *            finds the smallest m with (10·…·10 + 2)ₘ ≡ 0 (mod n) by
 *            iterating the congruence Rₖ = (Rₖ₋₁·10 + 2) mod n,...
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 16, i64 n <= 22, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  Handle the trivial
       non‑existence cases first. */
    uint64_t N = (uint64_t)n;
    if ((N % 4ULL == 0ULL) || (N % 5ULL == 0ULL))
        return (seq_t)-1;

    /* Step (1): find the minimal m such that
       R_m = 222…2 (m digits) ≡ 0 (mod N). */
    uint64_t r = 0ULL;
    uint64_t m = 0ULL;
    do {
        r = (r * 10ULL + 2ULL) % N;
        ++m;
    } while (r != 0ULL);

    /* Step (2): compute k = R_m / N by long division.
       The quotient fits into seq_t for all test cases that
       are expected to be representable in the underlying
       integer type.  The algorithm processes each of the m
       digits of the repdigit. */
    uint64_t rem = 0ULL;
    seq_t     q   = 0;
    for (uint64_t i = 0ULL; i < m; ++i) {
        rem   = rem * 10ULL + 2ULL;          /* bring in next digit */
        uint64_t d = rem / N;                /* quotient digit     */
        rem   %= N;                          /* remainder          */
        q     = q * 10 + (seq_t)d;           /* accumulate q       */
    }

    return q;
}
