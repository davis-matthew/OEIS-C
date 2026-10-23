/*
 * A123636  a(n) = 1 + 1*n + 1*n*2 + 1*n*2*(n-1) + 1*n*2*(n-1)*3 +
 *          1*n*2*(n-1)*3*(n-2) + ... + n!.
 * https://oeis.org/A123636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n-1} (∏_{i=1}^{k} f_i(n)) where f_i(n) = n -
 *            (i-1)/2 if i is odd = i/2 + 1 if i is even This reproduces
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=10, a(4)=37, … and runs in Θ(n)
 *            time
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases: n = 0 or 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t sum = (seq_t)1;          /* term for k = 0 */
    seq_t prod = (seq_t)1;         /* running product */

    for (idx_t k = 1; k <= n - 1; ++k)
    {
        seq_t factor;
        if (k & 1) /* odd k */
            factor = (seq_t)n - (seq_t)((k - 1) / 2);
        else       /* even k */
            factor = (seq_t)(k / 2 + 1);

        prod = prod * factor;
        sum  = sum + prod;
    }

    return sum;
}
