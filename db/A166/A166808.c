/*
 * A166808  Number of n X 5 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) - 2
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the value is undefined; return 0 */
    if (n < 1) return (seq_t)0;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= 5; ++i) {
        res = res * ((seq_t)n + (seq_t)i) / (seq_t)i;
    }
    return res - (seq_t)2;
}
