/*
 * A069572  Smallest n-digit number in which the k-th digit (from the right!)
 *          is a divisor of k, greater than 1 unless k has no single-digit
 *          divisor > 1.
 * https://oeis.org/A069572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = 10 * a(n-1) + d(n) where d(k) is the
 *            smallest single‑digit divisor of k greater than 1; if k has no
 *            such divisor (k = 1 or a prime > 9), then d(k) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = n; k > 0; --k) {
        int d = 1;              /* default digit for k having no divisor > 1 */
        for (int cand = 2; cand <= 9; ++cand) {
            if ((k % cand) == 0) {  /* k has a single‑digit divisor */
                d = cand;
                break;              /* minimal divisor found */
            }
        }
        result = result * 10 + (seq_t)d;
    }
    return result;
}
