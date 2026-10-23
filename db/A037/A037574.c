/*
 * A037574  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,2.
 * https://oeis.org/A037574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = sum_{i=0}^{n-1} p[i mod 3] * 9^(n-1-i), where p = {2,1,2} */
/* The base‑9 representation of a(n) consists of the first n digits of the repeating pattern 2,1,2. */

seq_t a(idx_t n)
{
    static const unsigned char pattern[3] = { 2, 1, 2 };
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * 9 + (seq_t)pattern[i % 3];
    }
    return res;
}
