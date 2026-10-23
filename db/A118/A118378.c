/*
 * A118378  a(n+1) = a(n)^2 - (-1)^n * binomial(n+2,2) with a(1) = 1.
 * https://oeis.org/A118378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 - (-1)^n * binomial(n+2,2) with a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t cur = 1; /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* binomial(i+2,2) = (i+2)*(i+1)/2  -- compute in seq_t */
        seq_t bin = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
        seq_t next = cur * cur;
        if (i & 1) {           /* i odd -> subtract(-1)*bin => add bin */
            next += bin;
        } else {               /* i even -> subtract(+1)*bin => subtract bin */
            next -= bin;
        }
        cur = next;
    }
    return cur;
}
