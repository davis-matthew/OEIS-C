/*
 * A106345  Diagonal sums of number triangle A106344.
 * https://oeis.org/A106345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (binomial(k, n-2k) mod 2)
 * Range:     u8 n <= 16381, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 119 terms through n = 9886 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - (k << 1);            /* m = n - 2*k   */
        if (m > k) continue;               /* binomial defined only when m ≤ k   */
        if ((m & (~k)) == 0) {             /* binomial(k, m) odd iff m & ~k == 0*/
            sum = sum + 1;                 /* increment count  */
        }
    }
    return sum;
}
