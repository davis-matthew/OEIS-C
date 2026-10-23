/*
 * A133824  Triangle whose rows are sequences of increasing and decreasing
 *          fourth powers: 1; 1,16,1; 1,16,81,16,1; ... .
 * https://oeis.org/A133824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = ((offset < r) ? (offset + 1) : (2 * r - 1 - offset))^4
 *            where r = floor(sqrt(n)) + 1 and offset = n - (r-1)^2
 * Range:     u8 n <= 11, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the flattened triangle of increasing and decreasing
   fourth powers.  See OEIS A133824. */

#include <stdint.h>

/* Compute floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static unsigned long long floor_sqrt_u64(unsigned long long n)
{
    unsigned long long x = 0, bit = 1ULL << 62;

    /* Bring the highest possible bit down to the range of sqrt(n). */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Determine the row number r (1-indexed). */
    unsigned long long r = floor_sqrt_u64((unsigned long long)n) + 1ULL;

    /* Compute the linear offset within this row. */
    unsigned long long prev_sq = (r - 1ULL) * (r - 1ULL);
    unsigned long long offset = n - prev_sq;

    /* Determine the appropriate fourth‑power index j. */
    unsigned long long j;
    if (offset < r)
        j = offset + 1ULL;
    else
        j = 2ULL * r - 1ULL - offset;

    /* Compute j^4 in seq_t. */
    seq_t jj = (seq_t)j;
    seq_t jj2 = jj * jj;
    seq_t jj4 = jj2 * jj2;
    return jj4;
}
