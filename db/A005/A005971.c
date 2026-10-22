/*
 * A005971  Partial sums of cubes of Lucas numbers.
 * https://oeis.org/A005971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000032(3*n+2)/2 + 3*(-1)^n*A000032(n-1) + 3/2 where
 *            A000032(n) is the Lucas number L(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t lucas(idx_t n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    
    seq_t l0 = 2, l1 = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t temp = l0 + l1;
        l0 = l1;
        l1 = temp;
    }
    return l1;
}

seq_t a(idx_t n) {
    seq_t l_3n_plus_2 = lucas(3 * n + 2);
    seq_t l_n_minus_1 = lucas(n - 1);
    
    seq_t result = l_3n_plus_2 + 3;
    if (n % 2 == 0) {
        result += 6 * l_n_minus_1;
    } else {
        result -= 6 * l_n_minus_1;
    }
    
    return result / 2;
}
