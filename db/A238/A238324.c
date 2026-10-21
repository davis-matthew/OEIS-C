/*
 * A238324  a(1) = 1 and a(n+1) = if a(n) > 2*n+1 then a(n)-2*n-1 else
 *          a(n)+2*n+1.
 * https://oeis.org/A238324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>=1: a(n+1) = (a(n) > 2*n+1 ? a(n)-2*n-1 : a(n)+2*n+1) */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for offset 0, return 0 */
        return 0;
    if (n == 1)
        return 1;         /* a(1) = 1 */

    seq_t val = 1;         /* a(1) */
    for (idx_t i = 1; i < n; ++i) { /* compute a(i+1) from a(i) */
        seq_t step = (seq_t)2 * (seq_t)i + (seq_t)1; /* 2*i + 1 */
        if (val > step)
            val = val - step;
        else
            val = val + step;
    }
    return val;
}
