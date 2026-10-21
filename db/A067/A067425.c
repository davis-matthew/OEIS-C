/*
 * A067425  Triangle with columns built from certain power sequences.
 * https://oeis.org/A067425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Let r = floor((sqrt(8n+1)-1)/2), * m = n - r(r+1)/2. * Then a(n)
 *            = 1 if r == m, * else (m+4) * (4*(m+1))^(r-m-1)
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 78, u64 n <= 212, u128 n <= 566,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

static inline idx_t row_of_index(idx_t n)
{
    /* Approximate using double; adjust to be exact. */
    long double d = sqrt((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) * 0.5L);
    /* Correct any error from rounding. */
    while (((__uint128_t)(r + 1) * (r + 2)) / 2 <= n) ++r;
    while (((__uint128_t)r * (r + 1)) / 2 > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t offset = n - (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t m = offset;

    if (r == m) return (seq_t)1;

    idx_t w = r - m - 1;
    idx_t base_int = 4 * (m + 1);

    seq_t pow_val = (seq_t)1;
    seq_t base_seq = (seq_t)base_int;

    for (idx_t i = 0; i < w; ++i)
        pow_val *= base_seq;

    return pow_val * (seq_t)(m + 4);
}
