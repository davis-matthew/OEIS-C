/*
 * A081857  Jacobsthal sequence (A001045) as represented in base 4.
 * https://oeis.org/A081857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081857:  Jacobsthal numbers represented in base‑4 and
   interpreted as a decimal string.  a(0)=0, a(1)=1.
   For n>0:
       if n is even, n=2k      → a(n) = decimal number consisting of k '1' digits
       if n is odd,  n=2k+1    → a(n) = decimal number consisting of (k-1) '2' digits followed by '3'.
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if ((n & (idx_t)1) == 0) {          /* n even: n = 2k */
        idx_t k = n >> 1;               /* k = n/2 */
        seq_t val = 0;
        for (idx_t i = 0; i < k; ++i)
            val = val * 10 + 1;
        return val;
    } else {                            /* n odd: n = 2k+1, k >= 1 */
        idx_t k = (n - 1) >> 1;          /* k = (n-1)/2 */
        seq_t val = 0;
        for (idx_t i = 0; i < k - 1; ++i)
            val = val * 10 + 2;
        val = val * 10 + 3;
        return val;
    }
}
