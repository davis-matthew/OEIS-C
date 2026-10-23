/*
 * A115268  Correlation triangle for floor((n+4)/4).
 * https://oeis.org/A115268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115268 – Correlation triangle for floor((n+4)/4) */
#if 0 /* The formula used by the program: */
/* a(n) is the flattened triangle T(r,k) where r = floor((sqrt(8n+1)-1)/2) and
 * k = n - r(r+1)/2.  Then
 *   T(r,k) = Σ_{j=0..min(k,r-k)} floor((k-j+4)/4) * floor((r-k-j+4)/4)
 */
#endif

#include <stdint.h>

/* Return the starting index of row r in the flattened triangle (type idx_t). */
static inline idx_t offset_row(idx_t r)
{
    /* compute r*(r+1)/2 using seq_t to avoid overflow of the multiplication */
    seq_t prod = (seq_t)r * ((seq_t)r + 1) / 2;
    return (idx_t)prod;
}

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
    {
        ++r;
    }

    /* column index within the row */
    idx_t k = n - offset_row(r);

    /* limits for the summation: j <= min(k, r-k) */
    idx_t limit = k < (r - k) ? k : (r - k);

    seq_t sum = 0;
    for (idx_t j = 0; j <= limit; ++j)
    {
        seq_t f1 = ((seq_t)(k - j) + 4) / 4;
        seq_t f2 = ((seq_t)(r - k - j) + 4) / 4;
        sum += f1 * f2;
    }

    return sum;
}
