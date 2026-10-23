/*
 * A182230  a(n) = a(n-1)+floor(a(n-2)/4) with a(0)=3, a(1)=4.
 * https://oeis.org/A182230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(a(n-2)/4); a(0)=3, a(1)=4
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 114, u64 n <= 231,
 *            u128 n <= 467, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)4;

    seq_t prev_prev = (seq_t)3;
    seq_t prev      = (seq_t)4;
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = prev + (prev_prev / 4);
        prev_prev = prev;
        prev      = current;
    }
    return prev;
}
