/*
 * A079249  Sum of r in all pairs (r,s), 0<=r<s, r+s divides n.
 * https://oeis.org/A079249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2)*∑_{d|n} ceil(d/2)*(ceil(d/2)-1)
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 for n=0; offset starts at 1 */

seq_t a(idx_t n)
{
    /* For safety, handle the non‑positive index. */
    if (n == 0) return 0;

    seq_t sum = 0;

    /* Enumerate divisors by iterating i up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            seq_t m = ((seq_t)i + 1) / 2;           /* ceil(i/2) */
            sum += m * (m - 1);

            /* corresponding divisor n/i, avoid double counting the square root */
            idx_t j = n / i;
            if (j != i)
            {
                seq_t m2 = ((seq_t)j + 1) / 2;     /* ceil(j/2) */
                sum += m2 * (m2 - 1);
            }
        }
    }

    /* The formula contains a factor 1/2; integer division is safe. */
    return sum / 2;
}
