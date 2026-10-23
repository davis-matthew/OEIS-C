/*
 * A127698  Sum of n-th triangular number and its reversal.
 * https://oeis.org/A127698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(n) = n*(n+1)/2 + reverse_decimal(n*(n+1)/2)
 * Range:     u8 n <= 13, u16 n <= 142, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_digits(seq_t v)
{
    seq_t rev = 0;
    while (v > 0) {
        rev = rev * 10 + v % 10;
        v   = v / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Compute the n-th triangular number */
    seq_t t = ((seq_t)n * ((seq_t)n + 1)) / 2;
    /* Add its decimal reversal */
    return t + reverse_digits(t);
}
