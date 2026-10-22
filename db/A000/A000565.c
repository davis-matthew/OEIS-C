/*
 * A000565  Number of discordant permutations.
 * https://oeis.org/A000565
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*C(n,7) + 448*C(n-1,7) + 1123*C(n-2,7) + 548*C(n-3,7) +
 *            131*C(n-4,7) - 88*C(n-5,7) + 6*C(n-6,7) - 12*C(n-7,7)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial(idx_t n, idx_t k) {
    if (k > n || k < 0) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 7) return 0;
    
    seq_t result = 0;
    result += 31 * binomial(n, 7);
    result += 448 * binomial(n - 1, 7);
    result += 1123 * binomial(n - 2, 7);
    result += 548 * binomial(n - 3, 7);
    result += 131 * binomial(n - 4, 7);
    result -= 88 * binomial(n - 5, 7);
    result += 6 * binomial(n - 6, 7);
    result -= 12 * binomial(n - 7, 7);
    
    return result;
}
