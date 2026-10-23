/*
 * A037746  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 2
 *   d(n) = 2,3,0,1 repeating
 *   a(n) = 6*a(n-1) + d(n)   (n ≥ 2)
 */
seq_t a(idx_t n)
{
    /* The pattern 2,3,0,1 repeats every 4 digits. */
    static const uint8_t pattern[4] = { 2, 3, 0, 1 };

    seq_t res = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        res = (res * (seq_t)6) + (seq_t)pattern[i & 3];
    }

    return res;
}
