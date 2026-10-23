/*
 * A010191  Continued fraction for sqrt(131).
 * https://oeis.org/A010191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 11; for n≥1, a(n) = [2,4,11,4,2,22][(n-1) % 6]
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;

    /* (n-1) is safe because n > 0 */
    idx_t r = (n - 1) % 6;

    switch (r)
    {
        case 0: return (seq_t)2;
        case 1: return (seq_t)4;
        case 2: return (seq_t)11;
        case 3: return (seq_t)4;
        case 4: return (seq_t)2;
        case 5: return (seq_t)22;
    }

    /* unreachable, keep compiler happy */
    return (seq_t)0;
}
