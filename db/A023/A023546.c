/*
 * A023546  Convolution of natural numbers >= 2 and A023531.
 * https://oeis.org/A023546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=2}^n k * A023531(n-k+1) where A023531(m) = 1 if m =
 *            k(k+1)/2 + k – 1 for some k ≥ 1, otherwise 0
 * Range:     u8 n <= 49, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    const idx_t limit = n - 1;          /* we need positions m ≤ n‑1 */

    for (idx_t k = 1; ; ++k) {
        /* position of the k‑th “1” in A023531 */
        idx_t ik = (k * k + 3 * k - 2) / 2;

        if (ik > limit)
            break;

        /* contribution of this “1” to a(n) */
        seq_t term = (seq_t)(n - ik + 1);
        sum += term;
    }
    return sum;
}
