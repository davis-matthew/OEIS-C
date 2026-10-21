/*
 * A050605  Column/row 2 of A050602: a(n) = add3c(n,2).
 * https://oeis.org/A050605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂((n+1)*(n+2)/2) = v₂(n+1) + v₂(n+2) – 1 (v₂(x) =
 *            exponent of 2 dividing x)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros of n+1 and n+2 (i.e. v₂) using builtin. */
    /* idx_t is uint64_t, so __builtin_ctzll is safe. */
    int tz1 = __builtin_ctzll(n + 1);
    int tz2 = __builtin_ctzll(n + 2);
    return (seq_t)(tz1 + tz2 - 1);
}
