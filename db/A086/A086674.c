/*
 * A086674  Sum of signed indices from Euler's pentagonal theorem (see
 *          A000041).
 * https://oeis.org/A086674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, k(3k−1)/2 < n} (+1)^{k−1} · (n − k(3k−1)/2) +
 *            Σ_{k≥1, k(3k+1)/2 < n} (+1)^{k−1} · (n − k(3k+1)/2) (The two
 *            sums use the same sign (+1 for odd k, −1 for even k).) This
 *            implements the definition of A086674 as a sum of signed
 *            differences with the generalized pentagonal numbers
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 the result is undefined; sequence is defined for n ≥ 1. */
    seq_t pos_sum = 0; /* sum of terms with + sign */
    seq_t neg_sum = 0; /* sum of terms with − sign */

    for (idx_t k = 1;; ++k) {
        seq_t kseq = (seq_t)k;

        /* Pentagonal number a_k = k(3k−1)/2   (k, −k paired). */
        seq_t a_k = kseq * ((seq_t)3 * kseq - (seq_t)1) / (seq_t)2;
        if (a_k >= n)            /* a_k is the smaller of the pair */
            break;              /* all further pentagonal numbers are ≥ n */

        /* Contribution of a_k.  Sign is + for odd k, – for even k. */
        if (k & 1)
            pos_sum += n - a_k; /* odd k → + sign */
        else
            neg_sum += n - a_k; /* even k → − sign */

        /* Pentagonal number b_k = k(3k+1)/2.  It is ≥ a_k. */
        seq_t b_k = kseq * ((seq_t)3 * kseq + (seq_t)1) / (seq_t)2;
        if (b_k < n) {
            if (k & 1)
                pos_sum += n - b_k;
            else
                neg_sum += n - b_k;
        }
    }

    /* The result is non‑negative: pos_sum ≥ neg_sum. */
    return pos_sum - neg_sum;
}
