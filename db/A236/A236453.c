/*
 * A236453  Number of length n strings on the alphabet {0,1,2} of the form 0^i
 *          1^j 2^k such that i,j,k>=0 and if i=1 then j=k.
 * https://oeis.org/A236453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 3 - (-1)^n)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * ((seq_t)n + 1) / 2;
    if ((n & 1) == 0)       /* n even */
        return base + 1;    /* a(n) = A000124(n) + 1 */
    else                    /* n odd  */
        return base + 2;    /* a(n) = A000124(n) + 2 */
}
