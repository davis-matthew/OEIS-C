/*
 * A006581  a(n) = Sum_{k=1..n-1} (k AND n-k).
 * https://oeis.org/A006581
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} (k AND n-k)
 * Range:     u8 n <= 36, u16 n <= 593, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t limit = (n - 1) / 2;
    
    for (idx_t k = 1; k <= limit; k++) {
        result += (seq_t)(k & (n - k));
    }
    
    result *= 2;
    if (n % 2 == 0) {
        result += (seq_t)(n / 2);
    }
    
    return result;
}
