/*
 * A000580  a(n) = binomial coefficient C(n,7).
 * https://oeis.org/A000580
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,7) = n*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6) / 7!
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 7) return 0;
    
    seq_t result = 1;
    seq_t sn = (seq_t)n;
    for (seq_t i = 1; i <= 7; i++) {
        result *= (sn - 7 + i);
        result /= i;
    }
    return result;
}
