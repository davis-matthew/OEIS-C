/*
 * A001968  v-pile positions of the 4-Wythoff game with i=3.
 * https://oeis.org/A001968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3/4)*(3+sqrt 5))
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    uint64_t k = (uint64_t)(4 * n + 3);
    uint64_t constant = 5236067977ULL;
    uint64_t divisor = 4000000000ULL;
    
    return (seq_t)(k * constant / divisor);
}
