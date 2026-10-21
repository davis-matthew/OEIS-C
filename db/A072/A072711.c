/*
 * A072711  Integers m such that the last digit of Fibonacci(m) is 9.
 * https://oeis.org/A072711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 60*((n-1)/8) + residues[(n-1)%8] where residues = {11,
 *            29, 31, 32, 38, 49, 52, 58}
 * Range:     u8 n <= 33, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* residues of indices m modulo 60 for which Fibonacci(m) ends in 9 */
    static const seq_t residues[8] = {11, 29, 31, 32, 38, 49, 52, 58};

    /* For n ≥ 1 compute (n-1)/8 and (n-1)%8 using idx_t arithmetic */
    idx_t nm1 = n - 1;                     /* n-1, safe because n≥1 */
    idx_t block = nm1 / 8;                 /* integer division */
    idx_t pos   = nm1 % 8;                 /* remainder */

    /* compute the actual term */
    seq_t term = (seq_t)60 * (seq_t)block + residues[pos];
    return term;
}
