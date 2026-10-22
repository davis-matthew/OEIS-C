/*
 * A006088  a(n) = (2^n + 2) a(n-1) (kissing number of Barnes-Wall lattice in
 *          dimension 2^n).
 * https://oeis.org/A006088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1}^{n} (2^i + 2), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_2(idx_t exp) {
    seq_t result = 1;
    seq_t base = 2;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / 2;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t power = power_of_2(i);
        result = result * (power + 2);
    }
    return result;
}
