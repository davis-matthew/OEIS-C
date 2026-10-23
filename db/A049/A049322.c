/*
 * A049322  Duplicate of A014557.
 * https://oeis.org/A049322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0..5)=0; for n>=6, a(n)=a(n-1)+2*(1 + floor((n-6)/2) +
 *            floor(floor((n-6)/2)/2) * ((floor((n-6)/2)+1)/2) )
 * Range:     i8 n <= 16, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n <= 5)               /* base values: a(0)=…=a(5)=0 */
        return 0;

    seq_t total = 0;           /* running sum of the sequence */

    /* add the differences for k=6 … n */
    for (idx_t k = 6; k <= n; ++k)
    {
        /* t = floor((k-6)/2)   */
        idx_t t_idx = (k - 6) >> 1;
        seq_t t = (seq_t)t_idx;

        /* b(t) = 1 + t + floor(t/2) * ((t+1)/2)  */
        seq_t b = 1 + t + (t / 2) * ((t + 1) / 2);

        /* diff = 2 * b(t)  */
        total += 2 * b;
    }

    return total;
}
