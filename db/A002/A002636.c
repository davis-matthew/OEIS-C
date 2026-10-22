/*
 * A002636  Number of ways of writing n as an unordered sum of at most 3
 *          nonzero triangular numbers.
 * https://oeis.org/A002636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Count of ways to write n as an unordered sum of at most 3
 *            nonzero triangular numbers
 * Range:     u8 n <= 1147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    
    // Generate triangular numbers up to N
    seq_t triangulars[1000];
    int count = 0;
    for (seq_t k = 1; count < 1000; k++) {
        seq_t tri = k * (k + 1) / 2;
        if (tri > N) break;
        triangulars[count++] = tri;
    }
    
    seq_t result = 0;
    
    // 0 terms: empty sum (only for n=0)
    if (N == 0) result++;
    
    // 1 term: n = T(a)
    for (int i = 0; i < count; i++) {
        if (triangulars[i] == N) {
            result++;
            break;
        }
    }
    
    // 2 terms: n = T(a) + T(b), a <= b
    for (int i = 0; i < count; i++) {
        for (int j = i; j < count; j++) {
            seq_t sum = triangulars[i] + triangulars[j];
            if (sum == N) {
                result++;
            } else if (sum > N) {
                break;
            }
        }
    }
    
    // 3 terms: n = T(a) + T(b) + T(c), a <= b <= c
    for (int i = 0; i < count; i++) {
        for (int j = i; j < count; j++) {
            for (int k = j; k < count; k++) {
                seq_t sum = triangulars[i] + triangulars[j] + triangulars[k];
                if (sum == N) {
                    result++;
                } else if (sum > N) {
                    break;
                }
            }
        }
    }
    
    return result;
}
