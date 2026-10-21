/*
 * A089410  Least common multiple of all cycle sizes (also the maximum cycle
 *          size) in range [A014137(n-1)..A014138(n-1)] of permutation
 *          A074679/A074680.
 * https://oeis.org/A089410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - 2 for n ≥ 4, a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 5
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)5;
    return ((seq_t)4 * (seq_t)n) - (seq_t)2;
}
