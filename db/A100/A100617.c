/*
 * A100617  There are n people in a room. The first half (i.e., floor(n/2)) of
 *          them leave, then 1/3 (i.e., floor of 1/3) of those remaining
 *          leave, then 1/4, then 1/5, etc.; sequence gives number who remain
 *          at the end.
 * https://oeis.org/A100617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = remaining number of people after iteratively removing
 *            floor(m/k) people for k = 2,3,4,… starting from m = n. The
 *            process stops when floor(m/k) = 0, i.e. k > m
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;      /* current number of people */
    seq_t k = 2;             /* current divisor */

    while (k <= m) {
        seq_t leave = m / k; /* people who leave at this step */
        if (!leave)          /* no one leaves; future steps are zero */
            break;
        m -= leave;          /* update the remaining count */
        ++k;
    }
    return m;
}
