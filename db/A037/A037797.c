/*
 * A037797  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base‑8 number whose digits are the first n terms of the
 *            repeating pattern 3,2,1,0. i.e. a(n) = (((3)*8+2)*8+1)*8+0 then
 *            repeat the cycle for all n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for A037797. */
seq_t a(idx_t n)
{
    /* digits of the period 3,2,1,0 */
    const seq_t pat[4] = {3, 2, 1, 0};
    seq_t res = 0;
    /* iterate over the first n digits */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* (i-1) & 3 selects the correct element in pat */
        res = (res << 3) + pat[(i - 1) & 3U];
    }
    return res;
}
