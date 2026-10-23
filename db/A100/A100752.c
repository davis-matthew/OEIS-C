/*
 * A100752  a(n) is the number of positive integers <= 10^n that are divisible
 *          by no prime exceeding 3.
 * https://oeis.org/A100752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3‑smooth integers ≤ 10^n
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t SCALE     = 1000000000000ULL; /* 1e12 to keep fixed‑point precision */
static const uint64_t LOG10_2   = 301029995664ULL;   /* round(log10(2) * SCALE)   */
static const uint64_t LOG10_3   = 477121254720ULL;   /* round(log10(3) * SCALE)   */

seq_t a(idx_t n)
{
    /* target = n * SCALE  (fixed‑point representation of n in base 10) */
    uint64_t target = (uint64_t)n * SCALE;
    uint64_t count = 0;

    for (uint64_t i = 0; ; ++i)
    {
        uint64_t i_part = i * LOG10_2;
        if (i_part > target) break;          /* 2^i already too large */

        uint64_t rem   = target - i_part;    /* remaining allowance for 3^j */
        uint64_t j_max = rem / LOG10_3;      /* largest j with 3^j ≤ 10^n / 2^i */

        /* include j = 0 ... j_max  (total j_max+1 values) */
        count += j_max + 1;
    }
    return (seq_t)count;
}
