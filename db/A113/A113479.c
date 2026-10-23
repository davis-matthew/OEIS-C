/*
 * A113479  Starting with the fraction 4/1 as the first term, a(n) is the
 *          numerator of the reduced fraction of the n-th term according to
 *          the rule: if n is even, multiply the previous term by n/(n+1);
 *          otherwise multiply the previous term by (n+1)/n.
 * https://oeis.org/A113479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(1)=4. For n>1, let the nth term be the numerator of the
 *            reduced fraction obtained by starting from 4/1 and, for k=2..n,
 *            multiplying by k/(k+1) when k is even and by (k+1)/k when k is
 *            odd
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safeguard for out‑of‑range */
    seq_t num = 4;                  /* numerator of current fraction */
    seq_t den = 1;                  /* denominator of current fraction */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t fnum, fden;
        if ((k & 1u) == 0)          /* k even: multiply by k/(k+1) */
        {
            fnum = (seq_t)k;
            fden = (seq_t)(k + 1);
        }
        else                        /* k odd: multiply by (k+1)/k */
        {
            fnum = (seq_t)(k + 1);
            fden = (seq_t)k;
        }

        /* Reduce the fraction before multiplication to keep values small */
        seq_t g = gcd_seq(fnum, den);
        fnum /= g;
        den /= g;

        g = gcd_seq(fden, num);
        fden /= g;
        num /= g;

        num *= fnum;
        den *= fden;
    }

    return num;   /* numerator of the reduced fraction */
}
