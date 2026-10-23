/*
 * A037536  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>=1: a(n)=3*a(n-1)+{1,2,1}[n-1 mod 3] */
/* Initial values: a(1)=1, a(2)=5, a(3)=16, … */

static const seq_t pattern[3] = {1, 2, 1};

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * 3 + pattern[i % 3];
    }
    return result;
}
