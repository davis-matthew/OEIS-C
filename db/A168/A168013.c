/*
 * A168013  a(n) = Sum of all divisors of all numbers < (n+1)^2.
 * https://oeis.org/A168013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{(n+1)^2-1} sigma(k) = * sum_{i=1}^{(n+1)^2-1}
 *            i * floor(((n+1)^2-1)/i)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 267, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) using a O(sqrt(n)) divisor–summation scheme. */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) corresponds to the first term.  */
    seq_t np1 = (seq_t)n + 1;          /* (n+1)                                    */
    seq_t M   = np1 * np1 - 1;         /* maximum index in the sigma sum          */

    seq_t ans = 0;
    seq_t i    = 1;

    while (i <= M)
    {
        seq_t q    = M / i;              /* floor(M / i)                            */
        seq_t last = M / q;              /* largest index with the same quotient     */

        seq_t count = last - i + 1;      /* number of terms in [i, last]            */
        seq_t sum   = (i + last) * count / 2;   /* arithmetic series sum   */

        ans += sum * q;                  /* contribution of this block              */

        i = last + 1;                    /* move to the next block                  */
    }

    return ans;
}
