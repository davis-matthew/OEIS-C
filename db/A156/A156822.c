/*
 * A156822  Prime factors of 17! listed with multiplicity.
 * https://oeis.org/A156822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth prime factor of 17! with multiplicity; 15 2's, 6
 *            3's, 3 5's, 2 7's, 1 11, 1 13, 1 17. Sequence length 29
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside of offset */

    /* cumulative counts up to each prime value */
    const idx_t up_to_2  = 15;
    const idx_t up_to_3  = up_to_2 + 6;   /* 21 */
    const idx_t up_to_5  = up_to_3 + 3;   /* 24 */
    const idx_t up_to_7  = up_to_5 + 2;   /* 26 */
    const idx_t up_to_11 = up_to_7 + 1;   /* 27 */
    const idx_t up_to_13 = up_to_11 + 1;  /* 28 */
    const idx_t up_to_17 = up_to_13 + 1;  /* 29 */

    if (n <= up_to_2)  return 2;
    if (n <= up_to_3)  return 3;
    if (n <= up_to_5)  return 5;
    if (n <= up_to_7)  return 7;
    if (n <= up_to_11) return 11;
    if (n <= up_to_13) return 13;
    if (n <= up_to_17) return 17;
    return 0; /* undefined for n > 29 */
}
