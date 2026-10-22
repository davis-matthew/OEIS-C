/*
 * A003079  One of the basic cycles in the x->3x-1 (x odd) or x/2 (x even)
 *          problem.
 * https://oeis.org/A003079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats the basic cycle [5, 14, 7, 20, 10] from the 3x-1
 *            mapping
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t cycle[] = {5, 14, 7, 20, 10};
    return cycle[n % 5];
}
