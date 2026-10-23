/*
 * A037627  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0.
 * https://oeis.org/A037627
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

/* Recurrence:  a(1) = 2;  for n > 1,
    a(n) = a(n-1) * 6 + d_{n-1},
    where the digit sequence d cycles 3, 0, 2, 3, 0, 2, ...
    (i.e. the missing digits after the initial 2 are 3, 0, 2 repeating). */
seq_t a(idx_t n)
{
    if (n == 0) /* offset below 1 is irrelevant */
        return 0;

    seq_t val = 2;                        /* a(1) */
    static const unsigned char cycle[3] = {3, 0, 2};

    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned int d = cycle[(i - 2) % 3];
        val = val * 6 + d;
    }
    return val;
}
