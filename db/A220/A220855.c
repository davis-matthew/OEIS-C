/*
 * A220855  Number of mappings by Struijk et al. Design Space Explorations
 *          with n actors and one implementation alternative.
 * https://oeis.org/A220855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1, a(3) = 39; for all n ≥ 2, n ≠ 3: a(n) = 11·n² – 19·n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          return 0;          /* out‑of‑range index */
    if (n == 1)         return 1;          /* special case */
    if (n == 3)         return 39;         /* special case */

    seq_t nn = (seq_t)n;
    return (nn * nn) * 11U - nn * 19U;
}
