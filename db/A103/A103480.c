/*
 * A103480  Row sums of A103462.
 * https://oeis.org/A103480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (n-k)^k + n
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = 1;                  /* 0^0 = 1, 0^e = 0 (e>0) handled below */
        seq_t base = (seq_t)k;
        idx_t exp   = n - k;
        while (exp > 0) {
            if (exp & 1) {
                term *= base;
            }
            /* avoid one unnecessary square when exp == 1 */
            if (exp > 1) {
                base *= base;
            }
            exp >>= 1;
        }
        sum += term;
    }
    sum += (seq_t)n;
    return sum;
}
