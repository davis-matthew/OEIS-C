/*
 * A005041  Positive integers k repeated ceiling(k/3) + 1 times.
 * https://oeis.org/A005041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Positive integers k repeated ceil(k/3) + 1 times. Find which k
 *            the index n falls into by computing cumulative run lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t cumsum = 0;
    idx_t k = 1;
    
    while (1) {
        idx_t run_length = (k + 2) / 3 + 1;
        if (n < cumsum + run_length) {
            return (seq_t)k;
        }
        cumsum += run_length;
        k++;
    }
}
