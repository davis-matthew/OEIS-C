/*
 * A095794  a(n) = A005449(n) - 1, where A005449 = second pentagonal numbers.
 * https://oeis.org/A095794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3*n-2)/2
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A095794: A005449(n) - 1 */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn = (seq_t)n;
    /* (n+1)*(3*n-2) is always even, so division by 2 is exact */
    seq_t result = (nn + 1) * (3 * nn - 2) / 2;
    return result;
}
