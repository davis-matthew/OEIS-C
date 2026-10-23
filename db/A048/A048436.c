/*
 * A048436  Take the first n numbers written in base 4, concatenate them, then
 *          convert from base 4 to base 10.
 * https://oeis.org/A048436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 4^(1+floor(log4(n))) + n, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1 */

    /* a(1) */
    seq_t res = 1;
    if (n == 1)
        return res;

    /* 4^1 initially – the number of base‑4 digits of 2 or 3 is 1 */
    seq_t pow4   = (seq_t)4;
    idx_t  next_thr = (idx_t)4;      /* where the digit length increases */

    for (idx_t cur = 2; cur <= n; ++cur)
    {
        if (cur == next_thr)          /* digit length grows for this cur */
        {
            pow4 *= (seq_t)4;         /* 4^(d+1) */
            next_thr *= (idx_t)4;    /* next threshold */
        }
        res = res * pow4 + (seq_t)cur;
    }
    return res;
}
