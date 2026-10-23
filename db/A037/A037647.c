/*
 * A037647  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; a(n) = 5 * a(n-1) + d(n) where d(n) = 3 if n%3==1, 0
 *            if n%3==2, 1 if n%3==0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t digit(idx_t i)
{
    idx_t r = i % 3;
    if (r == 1) return (seq_t)3;
    if (r == 2) return (seq_t)0;
    return (seq_t)1;          /* r == 0 */
}

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)5 + digit(i);
    }
    return val;
}
