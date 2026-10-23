/*
 * A169177  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   For 1 ≤ n ≤ 26:
 *     a(n) = 2 + 18 * (a(n-1) + a(n-2) + … + a(0))
 *   a(27) = 1 + 18 * (a(26) + a(25) + … + a(1)) - 171·a(0)
 *   For n ≥ 28:
 *     a(n) = 18 * (a(n-1) + a(n-2) + … + a(n-26)) - 171 · a(n-27)
 *   (Coefficients come from the rational generating function in OEIS A169177.)
 */

seq_t a(idx_t n)
{
    /* If n is 0 we can return immediately. */
    if (n == 0)
        return (seq_t)1;

    /* A ring buffer that stores the last 27 values of the sequence. */
    seq_t buf[27];
    buf[0] = (seq_t)1;           /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Sum the last up to 26 terms needed for the recurrence. */
        seq_t sum26 = (seq_t)0;
        idx_t maxk = (i < 26) ? i : 26;
        for (idx_t k = 1; k <= maxk; ++k)
            sum26 += buf[(i - k) % 27];

        if (i <= 27)   /* use the initial terms with the numerator contribution */
        {
            seq_t val = (i == 27) ? (seq_t)1 : (seq_t)2;  /* a_n from numerator */
            seq_t part = sum26 * (seq_t)18;                /* 18*sum of previous terms */
            val += part;
            if (i == 27)                                 /* subtract the 171*a(0) term */
                val -= (seq_t)171 * buf[0];
            buf[i % 27] = val;
        }
        else           /* recurrence for n ≥ 28 */
        {
            seq_t part = sum26 * (seq_t)18;
            seq_t sub  = (seq_t)171 * buf[(i - 27) % 27];
            buf[i % 27] = part - sub;
        }
    }

    return buf[n % 27];
}
