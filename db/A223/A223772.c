/*
 * A223772  Number of n X 3 0..1 arrays with rows and columns unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A223772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 9 n^5 + 145 n^4 + 375 n^3 + 3454 n^2 - 384 n +
 *            1440) / 720
 * Range:     u8 none, u16 n <= 3, u32 n <= 105, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; behaviour for n = 0 is arbitrary. */
    if (n == 0)
        return 0;

    /* Promote the index to seq_t early to avoid overflow in intermediate products. */
    seq_t nn = (seq_t)n;

    /* Compute powers of n as seq_t. */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    /* Build the numerator of the closed‑form polynomial. */
    seq_t num = n6
              + (seq_t)9   * n5
              + (seq_t)145 * n4
              + (seq_t)375 * n3
              + (seq_t)3454 * n2
              - (seq_t)384 * nn
              + (seq_t)1440;

    /* Divide by 720 (exact division). */
    seq_t result = num / (seq_t)720;

    return result;
}
