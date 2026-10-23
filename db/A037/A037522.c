/*
 * A037522  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) + d(n), where d(n) repeats 2,1,0 (digit(n) = 2
 *            if n≡1 mod 3, 1 if n≡2 mod 3, 0 otherwise), a(0)=0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit;
        idx_t m = i % 3;
        if (m == 1)          digit = 2;
        else if (m == 2)     digit = 1;
        else                 digit = 0;
        val = val * 5 + digit;
    }
    return val;
}
