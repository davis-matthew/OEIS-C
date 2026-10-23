/*
 * A033964  Trajectory of 1 under map n->13n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)= (a(n-1)/2 if a(n-1) is even else 13*a(n-1)+1) */
seq_t a(idx_t n)
{
    seq_t x = 1;
    for (idx_t i = 0; i < n; ++i) {
        if ((x & 1) == 0)
            x /= 2;
        else
            x = x * 13 + 1;
    }
    return x;
}
