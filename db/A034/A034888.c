/*
 * A034888  Number of digits in 3^n.
 * https://oeis.org/A034888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n*log10(3)) + 1
 * Range:     u8 n <= 534, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const long double LOG10_3 = 0.4771212547190450044807391357511L;

seq_t a(idx_t n)
{
    /* Compute floor(n*LOG10_3) with a tiny epsilon to avoid rounding‑down
       errors when the floating‑point product is marginally less than an
       integer. The truncation of a positive long double to uint64_t
       behaves like floor. */
    long double v = (long double)n * LOG10_3 + 1e-12L;
    uint64_t d = (uint64_t)v;               /* floor of the product   */
    return (seq_t)(d + 1);                  /* number of decimal digits */
}
