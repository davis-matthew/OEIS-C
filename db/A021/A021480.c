/*
 * A021480  Decimal expansion of 1/476.
 * https://oeis.org/A021480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit of the decimal expansion of 1/476 (decimal
 *            part only, indexed starting at 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = (idx_t)476;
    idx_t rem   = 1;   /* current remainder */
    idx_t digit = 0;

    /* Perform long‑division steps up to the n-th digit */
    for (idx_t i = 0; i <= n; ++i) {
        idx_t prod   = rem * (idx_t)10;
        digit       = prod / denom;
        rem         = prod % denom;
    }
    return (seq_t)digit;
}
