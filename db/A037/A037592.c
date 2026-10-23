/*
 * A037592  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A037592.
   The base‑6 digits are 1, 0, 3 repeating.
   a(n) is the integer whose first n base‑6 digits are this pattern.  */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t digit;
        switch (k % 3) {
            case 1: digit = 1; break;
            case 2: digit = 0; break;
            default: digit = 3; break;  /* k % 3 == 0 */
        }
        res = res * (seq_t)6 + digit;
    }
    return res;
}
