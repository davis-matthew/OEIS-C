/*
 * A018375  Divisors of 286.
 * https://oeis.org/A018375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 286 in increasing order
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t divisors[8];
    unsigned count = 0;
    for (unsigned long long i = 1; i <= 286ULL; ++i) {
        if (286ULL % i == 0ULL) {
            divisors[count++] = (seq_t)i;
        }
    }
    if (n == 0 || n > (idx_t)count) return 0;
    return divisors[n - 1];
}
