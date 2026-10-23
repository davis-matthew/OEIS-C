/*
 * A037104  Trajectory of 3 under map n->13n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3. For n>0, a(n) =
   a(n-1)/2 if a(n-1) is even, otherwise 13*a(n-1)+1
   (the trajectory of 3 under n→13n+1 for odd, n→n/2 for even). */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)3;
    if (n == 0)
        return x;
    for (idx_t i = 1; i <= n; ++i) {
        if ((x & (seq_t)1) == 0)
            x = x >> 1;
        else
            x = x * (seq_t)13 + (seq_t)1;
    }
    return x;
}
