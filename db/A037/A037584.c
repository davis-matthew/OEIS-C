/*
 * A037584  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1.
 * https://oeis.org/A037584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(2*5^n/3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the formula:
   a(1) = 3
   for n >= 2: a(n) = 5*a(n-1) + (n is even ? 1 : 3) */
seq_t a(idx_t n)
{
    /* Handle the first term directly.  The OEIS offset is 1. */
    if (n == 1) {
        return (seq_t)3;
    }

    seq_t val = (seq_t)3;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = val * (seq_t)5;      /* 5*a(i-1) */
        val += ( (i & (idx_t)1) == 0 ? (seq_t)1 : (seq_t)3 );
        /* add 1 when i is even, else add 3 when i is odd */
    }
    return val;
}
