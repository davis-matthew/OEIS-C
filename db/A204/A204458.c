/*
 * A204458  Odd numbers not divisible by 17.
 * https://oeis.org/A204458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-1 + 2*floor((n+7)/16)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                      /* convert idx_t to seq_t */
    seq_t val  = nn * 2 - 1;                    /* 2*n - 1  */
    seq_t add  = 2 * (seq_t)((n + 7) / 16);     /* 2*floor((n+7)/16) */
    return val + add;
}
