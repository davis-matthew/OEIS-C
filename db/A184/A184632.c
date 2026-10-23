/*
 * A184632  Floor(1/{(8+n^4)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1 * a(n) = ⌊n³ / 2⌋ for n ≥ 2 * This follows
 *            from floor(1/( { (8 + n⁴)^(1/4) } )) = ⌊n³/2⌋ for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* undefine for n == 0 – not part of the sequence */
    if (n == 0)
        return 0;

    /* the only exceptional first term */
    if (n == 1)
        return 1;

    /* compute ⌊n³/2⌋ in seq_t arithmetic, no overflow for the
     * supported seq_t sizes (uint64_t or BigInt). */
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;  /* n³  */
    return cube / 2;            /* ⌊n³/2⌋  */
}
