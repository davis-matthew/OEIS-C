/*
 * A055275  First differences of 9^n (A001019).
 * https://oeis.org/A055275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 8 * 9^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as defined for OEIS A055275. */
seq_t a(idx_t n)
{
    /* a(0) is defined specially. */
    if (n == 0)
        return 1;

    /* Compute 9^(n-1) by repeated multiplication. */
    seq_t result = 1;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)9;

    /* Multiply by the final factor 8. */
    result *= (seq_t)8;
    return result;
}
