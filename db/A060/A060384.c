/*
 * A060384  Number of decimal digits in n-th Fibonacci number.
 * https://oeis.org/A060384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(n*log10(phi) - log10(5)/2) + 1
 * Range:     u8 n <= 1221, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t SCALE             = 1000000000000000000ULL; /* 10^18 */
static const uint64_t LOG10_PHI_NUM    = 208987640249978734ULL;    /* log10(phi)*SCALE */
static const uint64_t LOG10_5_OVER_2_NUM = 349485002168009402ULL;   /* (log10(5)/2)*SCALE */

seq_t a(idx_t n)
{
    /* For n = 0 or 1, F_n = 0 or 1, each has one decimal digit. */
    if (n < 2)
        return 1;

    /* Compute n*log10(phi) with fixed‑point arithmetic. */
    unsigned __int128 prod = (unsigned __int128)n * LOG10_PHI_NUM;

    /* Subtract the constant term (log10(5)/2). */
    unsigned __int128 diff = prod - LOG10_5_OVER_2_NUM;

    /* Obtain the integer part and add 1 for the digit count. */
    uint64_t result = (uint64_t)(diff / SCALE) + 1;
    return (seq_t)result;
}
