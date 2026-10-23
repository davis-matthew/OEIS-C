/*
 * A101109  Number of sets of lists (sequences) of n labeled elements with k=3
 *          elements per list.
 * https://oeis.org/A101109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n%3 != 0; otherwise a(n) = n!/(n/3)! =
 *            ∏_{i=n/3+1}^{n} i
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 14, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* If n is not divisible by 3, there is no way to partition into lists of size 3 */
    if (n % 3 != 0) return 0;

    /* Compute the product of the integers from n/3+1 to n inclusive */
    seq_t result = 1;
    idx_t start = n / 3 + 1;
    for (idx_t i = start; i <= n; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
