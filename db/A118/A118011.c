/*
 * A118011  Complement of the Connell sequence (A001614); a(n) = 4*n -
 *          A001614(n).
 * https://oeis.org/A118011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + round(sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A118011: complement of the Connell sequence. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow on multiplication */
    seq_t nn = (seq_t)n;

    /* Compute floor(sqrt(2*n)) */
    seq_t two_n = nn * 2;
    seq_t s = floor_sqrt(two_n);

    /* Decide if the square root should be rounded up */
    seq_t s_sq = s * s;
    seq_t diff = two_n - s_sq;          /* 2*n - s^2   (non‑negative) */
    if (diff >= s + 1)                  /* round up if fractional part >= 0.5 */
        s += 1;

    /* Final formula: a(n) = 2*n + s */
    return nn * 2 + s;
}
