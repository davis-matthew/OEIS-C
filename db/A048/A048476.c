/*
 * A048476  a(n) = T(4,n), array T given by A048472.
 * https://oeis.org/A048476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*2^(n-1)+1 (equivalently
 *            a(n)=5*a(n-1)-8*a(n-2)+4*a(n-3))
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    seq_t val = (seq_t)5 * n;   /* 5*n */

    /* Multiply by 2^(n-1) by shifting left one bit at a time.
       This avoids undefined behaviour from shifting by >63 bits
       and works for all seq_t sizes. */
    for (idx_t i = 1; i < n; ++i)
        val <<= 1;          /* same as val = val * 2 */

    val += 1;                /* add the final +1 */
    return val;
}
