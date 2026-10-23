/*
 * A033965  Trajectory of 1 under map n->17n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Trajectory of 1 under map n->17n+1 if n is odd, otherwise n->n/2.
   a(0) = 1.  For n ≥ 0: a(n+1) = 17*a(n)+1 if a(n) odd, else a(n)/2. */

seq_t a(idx_t n)
{
    seq_t x = 1;                     /* start value a(0) */
    for (idx_t i = 0; i < n; ++i)    /* iterate n steps */
    {
        if ((x & 1) != 0)            /* odd? */
            x = x * 17 + 1;          /* odd step */
        else
            x = x / 2;               /* even step */
    }
    return x;
}
