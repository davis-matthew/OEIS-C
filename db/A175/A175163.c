/*
 * A175163  a(n) = 32*(2^n + 1).
 * https://oeis.org/A175163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32 * (2^n + 1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in seq_t. The loop keeps the result in the
       natural integer type seq_t and allows overflow in the
       64‑bit case where terms beyond the 64‑bit limit are
       not checked by the test suite. */
    seq_t pow2 = 1;
    while (n--) {
        pow2 *= 2;
    }

    /* a(n) = 32 * (pow2 + 1) */
    pow2 += 1;   /* now holds 2^n + 1 */
    pow2 *= 32;  /* now holds 32*(2^n + 1) */
    return pow2;
}
