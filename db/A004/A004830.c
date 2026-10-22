/*
 * A004830  Numbers that are the sum of at most 8 positive cubes.
 * https://oeis.org/A004830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A004830: a(n) = n-1 for n≤23, n for 24≤n≤238, n+1 for n>238 */

seq_t a(idx_t n) {
    if (n <= 23) {
        return (seq_t)(n - 1);
    } else if (n <= 238) {
        return (seq_t)n;
    } else {
        return (seq_t)(n + 1);
    }
}
