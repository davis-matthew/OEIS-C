/*
 * A006048  Number of entries in first n rows of Pascal's triangle not
 *          divisible by 3.
 * https://oeis.org/A006048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum_{i=0}^{n} f(i), where f(i) is the product of (d+1) for each base-3 digit d in i */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 0; i <= n; i++) {
        seq_t f_i = 1;
        idx_t temp = i;
        while (temp > 0) {
            idx_t digit = temp % 3;
            f_i *= (digit + 1);
            temp /= 3;
        }
        result += f_i;
    }
    return result;
}
