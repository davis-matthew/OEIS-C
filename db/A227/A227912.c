/*
 * A227912  Sum of all k from {1,...,n} such that the fractional part of n/k
 *          is contained in the closed interval [1/4, 3/4].
 * https://oeis.org/A227912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all k from {1,…,n} such that the fractional part
 *            of n/k lies in the closed interval [1/4, 3/4]. This is
 *            implemented by testing the remainder r = n mod k: r/k ∈ [1/4,
 *            3/4] ⇔ 4*r ≥ k and 4*r ≤ 3*k
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;           /* remainder of n divided by k   */
        if (r == 0)                /* integer division gives fraction 0 – skip   */
            continue;
        seq_t r4 = (seq_t)r * 4;   /* 4 * r                          */
        seq_t three_k = (seq_t)k * 3;
        if (r4 >= (seq_t)k && r4 <= three_k)
            sum += (seq_t)k;
    }
    return sum;
}
