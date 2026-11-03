/*
 * A075669  Sum of next n 8th powers.
 * https://oeis.org/A075669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=n*(n-1)/2+1}^{n*(n-1)/2+n} i^8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 18, u128 n <= 255,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 9750 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow8(seq_t x)
{
    /* Compute x^8 using 3 multiplications. */
    seq_t r = x * x;   /* x² */
    r = r * r;         /* x⁴ */
    r = r * r;         /* x⁸ */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* defined only for n ≥ 1 */

    seq_t n_seq = (seq_t)n;
    seq_t start = n_seq * (n_seq - 1) / 2 + 1;

    seq_t sum = 0;
    seq_t cur = start;

    for (idx_t i = 0; i < n; ++i)
    {
        sum += pow8(cur);
        ++cur;
    }
    return sum;
}
