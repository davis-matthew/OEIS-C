/*
 * A137913  Rows 2, 4, 6 of Mendeleyev-Seaborg (extended to 32 columns)
 *          periodic table elements.
 * https://oeis.org/A137913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 3 + n (n < 8) = 19 + (n-8) (8 ≤ n < 26) = 55 + (n-26) (26
 *            ≤ n < 58) For n ≥ 58 the sequence is not defined (return 0)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8ULL)
        return (seq_t)(3 + n);
    if (n < 26ULL)
        return (seq_t)(19 + (n - 8ULL));
    if (n < 58ULL)
        return (seq_t)(55 + (n - 26ULL));
    /* Undefined for larger n – return 0 */
    return (seq_t)0;
}
