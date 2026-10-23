/*
 * A037642  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1.
 * https://oeis.org/A037642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the value of the base‑7 number formed by the first * n
 *            digits of the repeating pattern 2,3,1. That is, * a(n) =
 *            Σ_{j=0}^{n-1} digit(j) * 7^{n-1-j}, * where digit(0)=2,
 *            digit(1)=3, digit(2)=1 and digit repeats every 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the repeating cycle of digits (2, 3, 1) */
    static const seq_t pat[3] = {2, 3, 1};

    seq_t value = 0;
    idx_t i;

    for (i = 0; i < n; ++i) {
        value = value * 7 + pat[i % 3];
    }

    return value;
}
