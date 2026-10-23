/*
 * A160664  a(n) = a(n-1) + A000203(n), a(0)=1.
 * https://oeis.org/A160664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^n k*⌊n/k⌋ ; recurrence
 *            a(n)=a(n-1)+A000203(n), a(0)=1
 * Range:     u8 n <= 17, u16 n <= 281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 1;                     /* a(0) */
    for (idx_t k = 1; k <= n; ++k) {
        sum += (seq_t)k * (seq_t)(n / k);
    }
    return sum;
}
