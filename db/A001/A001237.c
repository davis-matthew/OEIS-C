/*
 * A001237  Differences of reciprocals of unity.
 * https://oeis.org/A001237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)!^4 * Sum_{i=1..n+1} Sum_{j=1..i} Sum_{k=1..j}
 *            Sum_{l=1..k} 1/(ijkl)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n + 1;
    
    // Compute (m)!
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; i++) {
        fact *= (seq_t)i;
    }
    
    // Compute fact^4
    seq_t fact4 = fact * fact * fact * fact;
    
    // Compute the sum
    seq_t result = 0;
    for (idx_t i = 1; i <= m; i++) {
        for (idx_t j = 1; j <= i; j++) {
            for (idx_t k = 1; k <= j; k++) {
                for (idx_t l = 1; l <= k; l++) {
                    seq_t denom = (seq_t)i * (seq_t)j * (seq_t)k * (seq_t)l;
                    result += fact4 / denom;
                }
            }
        }
    }
    
    return result;
}
