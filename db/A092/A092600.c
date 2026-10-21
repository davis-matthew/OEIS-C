/*
 * A092600  Largest natural number x=a[n] for which sum of decimal digits
 *          exceeds x/n; for n=1, the case when no such number exists, a[1]=0
 *          was taken arbitrarily.
 * https://oeis.org/A092600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the largest integer x such that n*digit_sum(x) > x.
   For n = 1 there is no such x, so a(1) is defined to be 0. */

static inline seq_t digit_sum_seq(seq_t v)
{
    seq_t s = 0;
    while (v > 0)
    {
        s += v % (seq_t)10;
        v /= (seq_t)10;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety (n >= 1 in use) */

    /* Find the maximum number of decimal digits d that could satisfy
       10^(d-1) <= 9*d*n. */
    seq_t lower = 1;                     /* 10^(d-1) */
    idx_t d = 1;
    while (1)
    {
        seq_t rhs = (seq_t)9 * (seq_t)d * (seq_t)n;
        if (lower > rhs) break;          /* current d too large */
        d++;                             /* next digit length */
        lower *= (seq_t)10;              /* update to 10^d   */
    }
    idx_t dmax = d - 1;

    /* Upper bound for any candidate x:  x <= 9*dmax*n */
    seq_t upper = (seq_t)9 * (seq_t)dmax * (seq_t)n;

    for (seq_t x = upper; x > 0; --x)
    {
        seq_t s = digit_sum_seq(x);
        if (s * (seq_t)n > x) return x;
    }
    return 0;   /* occurs when n == 1 (no such number) */
}
