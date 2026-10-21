/*
 * A217923  F-block elements for Janet periodic table.
 * https://oeis.org/A217923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini
 * Formula:   a(n) = n + 56 for 1 <= n <= 14 (Lanthanides 57‑70) * a(n) = n +
 *            74 for 15 <= n <= 29 (Actinides 89‑103) * a(n) = 0 otherwise
 *            (sequence terminates after 29 terms)
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 14)
        return (seq_t)(n + 56);      /* 57,58,...,70 */
    if (n <= 29)
        return (seq_t)(n + 74);      /* 89,90,...,103 */
    return (seq_t)0;                 /* beyond the defined terms */
}
