/*
 * A006582  a(n) = Sum_{k=1..n-1} k XOR n-k.
 * https://oeis.org/A006582
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=1..floor((n-1)/2)} (k XOR (n-k))
 * Range:     u8 n <= 21, u16 n <= 317, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t limit = (n - 1) / 2;
    
    for (idx_t k = 1; k <= limit; k++) {
        result += (seq_t)(k ^ (n - k));
    }
    
    return result * 2;
}
