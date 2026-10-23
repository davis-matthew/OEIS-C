/*
 * A187673  Partial sums of the tricapped prism numbers A005920.
 * https://oeis.org/A187673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)(n+1)(9n^2+19n+12)/24, n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n) {
    /* Convert index to 128‑bit for safe intermediate calculations. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 term = (N + 2U) * (N + 1U) *
                             ((9U * N * N) + (19U * N) + 12U);
    term /= 24U;                    /* division is exact by the formula */
    return (seq_t)term;             /* truncate to the target bit‑width */
}
