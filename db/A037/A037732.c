/*
 * A037732  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; for n > 1, a(n) = 6 * a(n‑1) + d(n‑1) where the digit
 *            sequence d repeats 2, 1, 0, 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic digits in base 6: 2, 1, 0, 3 */
    static const seq_t period[4] = { (seq_t)2, (seq_t)1, (seq_t)0, (seq_t)3 };

    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * (seq_t)6 + period[i & 3];
    }
    return res;
}
