/*
 * A037121  Trajectory of 3 under map n -> 47n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3;
   a(n) = 47*a(n-1) + 1,   if a(n-1) is odd;
   a(n) = a(n-1)/2,        if a(n-1) is even. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)3;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((x % (seq_t)2) == (seq_t)1)          /* odd */
            x = (seq_t)47 * x + (seq_t)1;
        else                                     /* even */
            x = x / (seq_t)2;
    }
    return x;
}
