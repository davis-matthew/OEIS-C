/*
 * A010365  Class B multi-edge stars with n edges and 2 odd labeled roots.
 * https://oeis.org/A010365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 3 * C(n-6,1) + 16 * C(n-6,2) + 29 * C(n-6,3) + 124 *
 *            C(n-6,4) (C denotes binomial coefficient). For n < 6, a(n) = 0
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    idx_t k = n - 6;               /* k >= 0 */
    seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

    if (k >= 1) t1 = (seq_t)k;
    if (k >= 2) t2 = ((seq_t)k * (k - 1)) / 2;
    if (k >= 3) t3 = ((seq_t)k * (k - 1) * (k - 2)) / 6;
    if (k >= 4) t4 = ((seq_t)k * (k - 1) * (k - 2) * (k - 3)) / 24;

    seq_t res = 2
              + 3  * t1
              + 16 * t2
              + 29 * t3
              + 124 * t4;

    return res;
}
