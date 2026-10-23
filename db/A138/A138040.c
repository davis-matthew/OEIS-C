/*
 * A138040  Atomic number of the 2nd transition metal (group 4) in the n-th
 *          row of the periodic system of elements.
 * https://oeis.org/A138040
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(n) = 22 + 18*(n-4) for 4 ≤ n ≤ 5; a(n) = 32*n - 120 for n ≥ 6
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= 6)
        return 32 * (seq_t)n - (seq_t)120;
    if (n >= 4)
        return 22 + (seq_t)(n - 4) * 18;
    return 0; /* undefined for n < 4 */
}
