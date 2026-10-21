/*
 * A052293  Numbers of the form 11 + x^2 + x or 11 + 2*x^2.
 * https://oeis.org/A052293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth smallest of 11 + x*(x+1) or 11 + 2*x*x, x >= 0
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset is 1, a(1)=11 */

    idx_t i1 = 0;
    idx_t i2 = 0;
    idx_t count = 0;

    while (1)
    {
        seq_t v1 = (seq_t)11 + (seq_t)i1 * ((seq_t)i1 + 1);
        seq_t v2 = (seq_t)11 + ((seq_t)2 * (seq_t)i2 * (seq_t)i2);

        if (v1 < v2)
        {
            count++;
            if (count == n) return v1;
            i1++;
        }
        else if (v1 > v2)
        {
            count++;
            if (count == n) return v2;
            i2++;
        }
        else /* equal */
        {
            count++;
            if (count == n) return v1;
            i1++;
            i2++;
        }
    }
}
