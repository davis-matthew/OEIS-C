/*
 * A006523  Sum ((-1)^(i+1)*binomial(n,i)*2^i*(2*n-1)!,i=1..n) for n odd.
 * https://oeis.org/A006523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (4*n - 3)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = 4 * n - 3;
    seq_t result = 1;
    for (idx_t i = 2; i <= k; i++) {
        result *= (seq_t)i;
    }
    return result * 2;
}
