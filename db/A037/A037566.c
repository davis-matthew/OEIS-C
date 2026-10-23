/*
 * A037566  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2; for n > 1, a(n) = a(n-1) * 9 + d, where d = ((n-1) % 3
 *            == 2 ? 1 : 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = (( (i - 1) % 3 ) == 2) ? (seq_t)1 : (seq_t)2;
        res = res * (seq_t)9 + digit;
    }
    return res;
}
