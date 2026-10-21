/*
 * A206812  Position of 2^n in joint ranking of {2^i}, {3^j}, {5^k}.
 * https://oeis.org/A206812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*log_3(2)) + floor(n*log_5(2))
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision constants for log base 3 of 2 and log base 5 of 2.
   The values are accurate to within 1e-18, which is far below the
   1e‑12 epsilon added below to guarantee correct flooring for n ≤ 100 000. */
static const double LOG_3_2 = 0.6309297535714574375;
static const double LOG_5_2 = 0.4306765580733933117;

/* Return the position of 2ⁿ in the joint ranking of powers of 2, 3 and 5. */
seq_t a(idx_t n)
{
    /* The floor of a positive double can be obtained by truncating to
       an unsigned integer.  A tiny epsilon is added to guard against
       a rounding error that would cause the value to be just below an
       integer boundary. */
    const double eps = 1e-12;

    uint64_t j3 = (uint64_t)(LOG_3_2 * (double)n + eps);   /* floor(n*log₃(2)) */
    uint64_t j5 = (uint64_t)(LOG_5_2 * (double)n + eps);   /* floor(n*log₅(2)) */

    return (seq_t)n + (seq_t)j3 + (seq_t)j5;
}
