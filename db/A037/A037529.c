/*
 * A037529  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): integer whose base‑4 digits are the first n terms of the
   periodic sequence 1, 1, 2.  Recurrence:
   a(1) = 1;
   a(n) = 4 * a(n‑1) + d,   d = 1, 1, 2 repeated. */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset not used, but harmless */
        return 0;
    seq_t val = 1;   /* a(1) */
    if (n == 1)
        return val;

    static const unsigned char pattern[3] = {1, 1, 2};

    for (idx_t i = 2; i <= n; ++i) {
        unsigned char d = pattern[(i - 1) % 3];
        val = val * (seq_t)4 + (seq_t)d;
    }
    return val;
}
