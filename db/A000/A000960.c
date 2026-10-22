/*
 * A000960  Flavius Josephus's sieve: Start with the natural numbers; at the
 *          k-th sieving step, remove every (k+1)-st term of the sequence
 *          remaining after the (k-1)-st sieving step; iterate.
 * https://oeis.org/A000960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   start with n^2, then successively round down to highest multiple
 *            of n-1, n-2, ..., 1
 * Range:     u8 n <= 15, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n * (seq_t)n;
    
    for (seq_t divisor = (seq_t)n - 1; divisor > 0; divisor--) {
        seq_t quotient = result / divisor;
        if (result % divisor == 0) {
            quotient--;
        }
        result = quotient * divisor;
    }
    
    return result;
}
