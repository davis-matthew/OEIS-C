/*
 * A037801  Number of i such that d(i)<d(i-1), where Sum{d(i)*3^i:
 *          i=0,1,...,m} is base 3 representation of n.
 * https://oeis.org/A037801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i such that d(i) < d(i-1) in the base‑3
 *            representation of n, where n = Σ d(i)·3^i for i = 0…m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Get the least significant digit first. */
    idx_t prev = n % 3;
    n = n / 3;
    /* Process remaining digits. */
    while (n > 0) {
        idx_t curr = n % 3;
        if (curr < prev) ++count;
        prev = curr;
        n = n / 3;
    }
    return count;
}
