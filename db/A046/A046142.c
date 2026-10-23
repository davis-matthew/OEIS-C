/*
 * A046142  Haüy rhombic dodecahedral numbers.
 * https://oeis.org/A046142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n - 1)*(8*n^2 - 14*n + 7)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 645, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1 – behaviour for n<1 is irrelevant */

    seq_t N = (seq_t)n;

    /* Compute 2*n - 1  and  8*n^2 - 14*n + 7  */
    seq_t part1 = ((seq_t)2 * N) - (seq_t)1;
    seq_t part2 = ((seq_t)8 * N * N) - ((seq_t)14 * N) + (seq_t)7;

    return part1 * part2;
}
