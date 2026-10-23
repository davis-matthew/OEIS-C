/*
 * A037598  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=0}^{n-1} d_k * 5^(n-1-k), where d_k = {1,3,0}
 *            repeating. Recurrence: a(1) = 1; a(n) = 5 * a(n-1) + (n-1 mod 3
 *            == 0 ? 1 : (n-1 mod 3 == 1 ? 3 : 0))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t digit;
        switch (i % 3) {
            case 0: digit = (seq_t)1; break;
            case 1: digit = (seq_t)3; break;
            default: digit = (seq_t)0; break;
        }
        val = val * (seq_t)5 + digit;
    }
    return val;
}
