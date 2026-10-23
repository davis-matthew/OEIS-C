/*
 * A037666  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,1,0.
 * https://oeis.org/A037666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the periodic digit sequence 3,1,0.
   Each a(n) is the decimal number formed by the first n digits of that sequence. */
seq_t a(idx_t n)
{
    static const int digits[3] = {3, 1, 0};
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int idx = (int)((i - 1) % 3);
        val = val * 10 + (seq_t)digits[idx];
    }
    return val;
}
