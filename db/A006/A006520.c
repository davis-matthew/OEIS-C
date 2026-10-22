/*
 * A006520  Partial sums of A006519.
 * https://oeis.org/A006520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + Sum_{k>=1} 2^(k-1)*floor(n/2^k)
 * Range:     u8 n <= 63, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n;
    for (int k = 1; k < 64; k++) {
        idx_t divisor = (idx_t)1 << k;
        if (divisor > n) break;
        seq_t multiplier = (seq_t)1 << (k - 1);
        result += multiplier * ((seq_t)(n / divisor));
    }
    return result;
}
