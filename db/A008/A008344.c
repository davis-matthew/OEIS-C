/*
 * A008344  a(1)=0; thereafter a(n+1) = a(n) - n if a(n) >= n otherwise a(n+1)
 *          = a(n) + n.
 * https://oeis.org/A008344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 8784, u32 n <= 8784, u64 n <= 8784,
 *            u128 n <= 8784, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008344
   Formula: a(1)=0; for n≥1, a(n+1)=a(n)-n if a(n)≥n else a(n)+n
   (equivalently: a(n) = (a(n-1)+(n-1)) mod 2*(n-1)) */

seq_t a(idx_t n)
{
    /* a(1) is the first term */
    if (n <= 1) return 0;

    seq_t val = 0;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t step = (seq_t)(i - 1);
        if (val >= step)
            val -= step;   /* a(n) >= n-1  →  a(n+1) = a(n) - (n-1) */
        else
            val += step;   /* a(n) <  n-1  →  a(n+1) = a(n) + (n-1) */
    }
    return val;
}
