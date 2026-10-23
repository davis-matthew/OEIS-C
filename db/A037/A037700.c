/*
 * A037700  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037700: a(n) is the integer whose base‑9 digits are the first n terms of the periodic sequence 1,2,3,0. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    const unsigned int pat[4] = {1, 2, 3, 0};

    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 9 + pat[i & 3];
    }
    return result;
}
