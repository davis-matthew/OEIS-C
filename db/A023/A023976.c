/*
 * A023976  First bit in fractional part of binary expansion of 9th root of n.
 * https://oeis.org/A023976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the fractional part of n^(1/9) >= 1/2, otherwise 0
 * Range:     u8 none, u16 n <= 102, u32 n <= 102, u64 n <= 102, u128 n <= 102,
 *            big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute x^9 for an arbitrary‑precision integer type seq_t */
static seq_t pow9_seqt(seq_t x)
{
    seq_t x2 = x * x;          /* x^2   */
    seq_t x4 = x2 * x2;        /* x^4   */
    seq_t x8 = x4 * x4;        /* x^8   */
    return x8 * x;             /* x^9   */
}

seq_t a(idx_t n)
{
    /* 1) find m = floor(n^(1/9))  */
    idx_t lo = 0;
    idx_t hi = 1;
    seq_t sn = (seq_t)n;          /* cast n once for cheap comparisons */

    /* find an upper bound where hi^9 > n */
    while (pow9_seqt((seq_t)hi) <= sn)
        hi <<= 1;                 /* double hi */

    /* binary search for the greatest hi with hi^9 <= n */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (pow9_seqt((seq_t)mid) <= sn)
            lo = mid;
        else
            hi = mid - 1;
    }
    seq_t m = (seq_t)lo;          /* m = floor(n^(1/9)) */

    /* 2) test whether fractional part >= 0.5
       a(n) = 1 iff n*512 >= (2*m+1)^9  */
    seq_t left  = sn * ((seq_t)512);   /* n*2^9  (512 = 2^9) */
    seq_t r     = m * 2 + 1;           /* 2*m+1   */
    seq_t right = pow9_seqt(r);

    return (left >= right) ? 1 : 0;
}
