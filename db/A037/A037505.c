/*
 * A037505  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the integer whose base‑4 representation has its first n
 *            digits equal to the periodic sequence 1,2,0,1,2,0… i.e. a(1)=1,
 *            a(2)=6, a(3)=24, a(4)=97, …
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* Determine the next digit: 1, 2, or 0 in a repeating pattern. */
        int r = (int)(i % 3);          /* r in {0,1,2} */
        seq_t digit = (r == 0) ? (seq_t)1 :
                      (r == 1) ? (seq_t)2 : (seq_t)0;
        /* Append the digit: shift left by one base‑4 digit and add. */
        val = val * (seq_t)4 + digit;
    }
    return val;
}
