/*
 * A005940  The Doudna sequence: write n-1 in binary; power of prime(k) in
 *          a(n) is # of 1's that are followed by k-1 0's.
 * https://oeis.org/A005940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabf, look
 * Formula:   a(n) = f(n-1, 1, 1) where f(n, i, x) = x if n = 0, = f(n/2, i+1,
 *            x) if n is even, = f((n-1)/2, i, x*prime(i)) otherwise
 * Range:     u8 n <= 56, u16 n <= 484, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    idx_t i = 1;
    
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };
    
    while (m > 0) {
        if (m % 2 == 1) {
            result *= (seq_t)primes[i - 1];
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    
    return result;
}
