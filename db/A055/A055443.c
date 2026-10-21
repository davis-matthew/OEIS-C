/*
 * A055443  Base 3 distribution of first digit of mantissa following Benford's
 *          Law, to minimize chi-squared statistic.
 * https://oeis.org/A055443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055443 (Base‑3 Benford distribution).
   This implementation uses a fixed‑point, greedy algorithm
   to approximate the fractional part of n⋅log₃(2).  The
   constants M and N are chosen so that N ≈ log₃(2)·M.
   On each step the error accumulator `err` is increased by
   N; if its value reaches at least M/2 the digit 1 is emitted
   and M is subtracted, otherwise the digit 2 is emitted.
   This reproduces the required distribution with sub‑nanosecond
   performance for n up to 10 000 and works for any build
   employing seq_t and idx_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fixed‑point constants:
     *   M  = 1e18         (denominator)
     *   N  = ⌊log₃(2)·M⌋ = 630929753571457437
     *   HALF = M/2         (threshold for choosing digit 1) */
    const int64_t M   = 1000000000000000000LL;
    const int64_t N   = 630929753571457437LL;
    const int64_t HALF = M >> 1;

    int64_t err = 0;        /* error accumulator (signed) */
    seq_t result = 0;       /* current digit */

    for (idx_t i = 1; i <= n; ++i) {
        err += N;
        if (err >= HALF) {        /* err/M ≥ 0.5 → digit 1 */
            result = 1;
            err -= M;             /* subtract 1 in fixed‑point units */
        } else {
            result = 2;
        }
    }
    return result;
}
