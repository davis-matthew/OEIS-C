/*
 * A123279  a(n) = product of the first n integers from among those positive
 *          integers which are coprime to n.
 * https://oeis.org/A123279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n integers that are coprime to n
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t count = 0;
    idx_t k = 1;

    /* Simple Euclidean algorithm for gcd on small integers */
    while (count < n) {
        idx_t a_ = k, b_ = n;
        while (b_ != 0) {
            idx_t t = a_ % b_;
            a_ = b_;
            b_ = t;
        }
        if (a_ == 1) {
            prod = prod * (seq_t)k;
            count++;
        }
        k++;
    }
    return prod;
}
