/*
 * A037633  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n‑1) * 5 + digit_n, a(1) = 2, where digit_n = {2,1,3}
 *            repeating (digit_1 = 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The pattern repeats every 3 digits: 2, 1, 3. */
    static const unsigned pattern[3] = {2, 1, 3};

    /* Since the offset is 1, n must be >= 1.  For safety,
       return 0 when n == 0, although this situation should not
       occur in a valid OEIS lookup. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)2;           /* a(1) */
    if (n == 1)
        return result;

    /* Compute next terms iteratively. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Digit index for the i-th term: pattern[(i-1) % 3] */
        unsigned d = pattern[(i - 1) % 3];
        result = result * (seq_t)5 + (seq_t)d;
    }
    return result;
}
