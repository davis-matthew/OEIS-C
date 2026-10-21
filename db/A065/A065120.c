/*
 * A065120  Highest power of 2 dividing A057335(n).
 * https://oeis.org/A065120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = number of times a right‑shift of n produces a power of two.
 * Recurrence: a(0) = 0, a(2n+1) = a(n),
 *             a(2n)   = a(n) + 1 if n is a power of two, otherwise a(2n) = a(n).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;
    for (idx_t m = n; m != 0; m >>= 1) {
        seq_t v = (seq_t)m;
        if ( (v & (v - 1)) == 0 )   /* v is a power of two */
            ++count;
    }
    return count;
}
