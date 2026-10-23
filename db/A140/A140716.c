/*
 * A140716  Blocky integers, i.e., integers m > 1 such that there is a run of
 *          m consecutive integer squares the average of which is a square.
 * https://oeis.org/A140716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n - 2 + 3*(-1)^n (n >= 1)
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent explicit formula:  a(n) = 12*n - 5  if n is odd,    */
/*                                 a(n) = 12*n + 1  if n is even. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0)  /* even index */
        return N * 12 + 1;
    else              /* odd index */
        return N * 12 - 5;
}
