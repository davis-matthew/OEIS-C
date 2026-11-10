/*
 * A220695  Complement of A220655.
 * https://oeis.org/A220695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2 write n - 1 = 2! + 3! + ... + (j-1)! + k
 *            with 1 <= k <= j!; then a(n) = j! + Sum_{i=1..j-1} i! + k - 1.
 *            These are the numbers that A220655 skips: between its block for
 *            m = j-1 and its block for m = j there are exactly j! missing
 *            integers
 * Range:     u8 n <= 136, u16 n <= 5913, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Validated: standard, pass, 75 terms through n = 5767 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    seq_t k = (seq_t)(n - 1);
    seq_t s = 1;   /* S(j-1) = Sum_{i=1..j-1} i!, starting at j = 2 */
    seq_t f = 1;   /* j! */
    for (idx_t j = 2; ; ++j) {
        f = f * (seq_t)j;                 /* j! */
        if (k <= f) {
            return f + s + (k - 1);
        }
        k -= f;
        s += f;                           /* S(j) = S(j-1) + j! */
    }
}
