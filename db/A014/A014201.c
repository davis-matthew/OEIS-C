/*
 * A014201  Number of solutions to x^2 + x*y + y^2 <= n excluding (0,0).
 * https://oeis.org/A014201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * Σ_{k=0}^{⌊n/3⌋} (⌊n/(3k+1)⌋ - ⌊n/(3k+2)⌋)
 * Range:     u8 n <= 72, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t maxk   = n / 3;
    seq_t  sum   = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t den1 = 3 * k + 1;
        idx_t den2 = 3 * k + 2;
        seq_t q1   = n / den1;
        seq_t q2   = n / den2;
        sum += q1 - q2;
    }
    return sum * 6;
}
