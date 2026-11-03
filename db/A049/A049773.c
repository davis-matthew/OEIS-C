/*
 * A049773  Triangular array T read by rows: if row n is r(1),...,r(m), then
 *          row n+1 is 2r(1)-1,...,2r(m)-1,2r(1),...,2r(m).
 * https://oeis.org/A049773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, nice, look
 * Formula:   a(n) = reverse_bits( n - 2^{⌊log₂ n⌋} , ⌊log₂ n⌋ ) + 1 where
 *            reverse_bits(x, k) reverses the lowest k bits of x
 * Range:     u8 n <= 510, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 98 terms through n = 7206 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute the position of the most significant bit (1‑based). */
static inline int bit_len(idx_t v)
{
    int len = 0;
    while (v) { v >>= 1; ++len; }
    return len;            /* len >= 1 for v >= 1 */
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    int R = bit_len(n);                     /* row number */
    idx_t start = ((idx_t)1) << (R - 1);     /* first index of that row */
    idx_t offset0 = n - start;              /* 0‑based index within the row */

    /* reverse the lower (R-1) bits of offset0 */
    idx_t rev = 0;
    idx_t t   = offset0;
    for (int i = 0; i < R - 1; ++i) {
        rev = (rev << 1) | (t & 1);
        t >>= 1;
    }
    return (seq_t)(rev + 1);                /* values are 1‑based */
}
