/*
 * A037541  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037541:  a(n) is the integer whose base‑8 digits are the first n
 * digits of the repeating pattern 1,2,1.  Equivalent recurrence:
 *
 *   a(0) = 0
 *   a(n) = a(n-1) * 8 + d(n)   (n >= 1)
 *   where d(n) = 2 if n mod 3 == 2 else 1
 *
 * This implementation uses no dynamic allocation and is O(n) per call. */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = (i % 3 == 2) ? ((seq_t)2) : ((seq_t)1);
        val = val * ((seq_t)8) + digit;
    }
    return val;
}
