/*
 * A037591  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; a(n)=5*a(n-1)+{1,0,3}[ (n-1)%3 ] (for n≥2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not defined for offset 0 */

    seq_t result = (seq_t)0;
    const seq_t five  = (seq_t)5;

    for (idx_t i = 0; i < n; ++i)
    {
        int d = 0;
        switch (i % 3)
        {
            case 0: d = 1; break;
            case 1: d = 0; break;
            default: d = 3; break;
        }
        result = result * five + (seq_t)d;
    }
    return result;
}
