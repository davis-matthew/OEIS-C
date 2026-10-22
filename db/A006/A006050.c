/*
 * A006050  Smallest number of additive persistence n.
 * https://oeis.org/A006050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = 2*10^((a(n-1)-1)/9) - 1, a(0) = 0, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10(seq_t exp) {
    seq_t result = 1;
    seq_t base = 10;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 10;
    
    seq_t val = 10;
    for (idx_t i = 2; i <= n; i++) {
        seq_t exp = (val - 1) / 9;
        val = 2 * pow10(exp) - 1;
    }
    
    return val;
}
