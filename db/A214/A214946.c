/*
 * A214946  Number of squarefree words of length 7 in an (n+1)-ary alphabet.
 * https://oeis.org/A214946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + n^6 - 4*n^5 - 3*n^4 + 5*n^3 + 2*n^2 - 2*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 for safety. */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;

    /* Compute successive powers of n. */
    seq_t n2 = sn * sn;          /* n^2   */
    seq_t n3 = n2 * sn;          /* n^3   */
    seq_t n4 = n3 * sn;          /* n^4   */
    seq_t n5 = n4 * sn;          /* n^5   */
    seq_t n6 = n5 * sn;          /* n^6   */
    seq_t n7 = n6 * sn;          /* n^7   */

    /* Positive part: n^7 + n^6 + 5*n^3 + 2*n^2 */
    seq_t pos = n7 + n6 + 5 * n3 + 2 * n2;

    /* Negative part: 4*n^5 + 3*n^4 + 2*n */
    seq_t neg = 4 * n5 + 3 * n4 + 2 * sn;

    /* pos ≥ neg for all n ≥ 1, so unsigned subtraction is safe. */
    return pos - neg;
}
