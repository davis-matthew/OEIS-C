/*
 * A001730  a(n) = n!/6!.
 * https://oeis.org/A001730
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / 6!
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    seq_t result = 1;
    for (idx_t i = 7; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}
