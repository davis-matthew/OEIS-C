/*
 * A133221  A001147 with each term repeated.
 * https://oeis.org/A133221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*floor(n/2)-1)!! = product of odd integers ≤ n-1
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of sequence A133221 (A001147 with each term repeated). */
seq_t a(idx_t n)
{
    /* For n = 0,1,2,3 the sequence is defined to be 1. */
    if (n < 4)
        return (seq_t)1;

    /* floor(n/2) computed on idx_t; all operations stay in unsigned arithmetic. */
    idx_t half = n / 2;

    /* k = 2*floor(n/2)-1 is the largest odd integer ≤ n-1. */
    seq_t k = (seq_t)(2 * half - 1);

    /* Compute the product of all odd integers from 1 to k. */
    seq_t result = (seq_t)1;
    for (seq_t i = (seq_t)1; i <= k; i += (seq_t)2)
        result *= i;

    return result;
}
