/*
 * A008759  Expansion of (1+x^16)/(1-x)/(1-x^2)/(1-x^3).
 * https://oeis.org/A008759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + p(n-16) where p(n) counts partitions of n into 1,
 *            2, and 3. p(n) = Σ_{c=0}^{⌊n/3⌋} (⌊(n−3c)/2⌋ + 1)
 * Range:     u8 n <= 43, u16 n <= 632, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t c = 0; c <= n / 3; ++c) {
        idx_t rem = n - 3 * c;              /* remaining after using c parts of size 3 */
        seq_t add = ((seq_t)rem) / 2 + 1;   /* number of solutions to a + 2b = rem */
        total += add;
    }
    if (n >= 16) {                         /* add partitions of n-16 (if any) */
        idx_t m = n - 16;
        for (idx_t c = 0; c <= m / 3; ++c) {
            idx_t rem = m - 3 * c;
            seq_t add = ((seq_t)rem) / 2 + 1;
            total += add;
        }
    }
    return total;
}
