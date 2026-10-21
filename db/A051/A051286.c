/*
 * A051286  Whitney number of level n of the lattice of the ideals of the
 *          fence of order 2n.
 * https://oeis.org/A051286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k, k)^2 implements the sum of
 *            squares of “restricted” binomial coefficients. The calculation
 *            uses only integer arithmetic and works for any seq_t type (fixed
 *            width or arbitrary precision)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For k = 0 we have C(n,0) = 1, so start with this value. */
    seq_t sum = 1;          /* a(0) = 1 guarantees correct start. */
    seq_t A   = 1;          /* Current binomial C(n-k, k) */
    idx_t  maxk = n / 2;    /* Only terms where k ≤ n−k are non‑zero */

    /* Incrementally compute the successive binomial coefficients
       using the recurrence
          A_{k} = A_{k−1} * (n−2(k−1)) * (n−2(k−1)−1) / [ (n−(k−1)) * k ]
       (valid for k ≥ 1). */
    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t num   = (seq_t)(n - 2*(k-1)) * (seq_t)(n - 2*(k-1) - 1);
        seq_t denom = (seq_t)(n - (k-1)) * (seq_t)k;
        A = (A * num) / denom;    /* exact division by construction */
        sum += A * A;
    }
    return sum;
}
