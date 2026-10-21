/*
 * A075999  Product{[n/k + 1/2]: k=1,2,...,n}, where [x + 1/2] denotes the
 *          integer nearest to x.
 * https://oeis.org/A075999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} round(n/k) where round(x) is the integer
 *            nearest to x. In integer arithmetic this is given by floor((n +
 *            k/2)/k). The function uses O(n) multiplications and works for
 *            any seq_t (built‑in or big‑int)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        /* nearest integer to n/k: floor((n + k/2)/k)  */
        idx_t half = k / 2;                    /* k/2 in unsigned  */
        seq_t factor = ((seq_t)n + (seq_t)half) / (seq_t)k;
        prod *= factor;
    }
    return prod;
}
