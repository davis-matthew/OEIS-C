/*
 * A190941  Partial sums of A190592.
 * https://oeis.org/A190941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{i=0..n} (maximal digit in base‑3 expansion of i)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)0; i <= n; ++i) {
        idx_t x = i;
        int lmax = 0;
        if (x == 0) {
            /* lmax remains 0 for i = 0 */
        } else {
            while (x > 0) {
                int d = (int)(x % 3);
                if (d > lmax) {
                    lmax = d;
                    if (lmax == 2) break;   /* 2 is the maximum base‑3 digit */
                }
                x /= 3;
            }
        }
        sum += (seq_t)lmax;
    }
    return sum;
}
