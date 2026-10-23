/*
 * A137506  a(2*n+1) = 141 + 124*n, a(2*n+2) = |a(2*n) - 24| with a(2)=59,
 *          thus a(4,6,8,...) = 35,11,13,11,13...
 * https://oeis.org/A137506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = 62*n + 79 if n is odd a(n) = 83 - 12*n if n is even and n
 *            <= 6 a(n) = 12 + (n/2%2 ? -1 : 1) if n is even and n >= 6
 *            (Surely a(n) = a(n-2)+a(n-4)-a(n-6) for n>=11, but the above
 *            closed form works for all n >= 1.)
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 2 == 1)                /* odd */
        return (seq_t)62 * (seq_t)n + (seq_t)79;
    else if (n <= 6)               /* even, small n */
        return (seq_t)83 - (seq_t)12 * (seq_t)n;
    else                           /* even, large n */
    {
        seq_t base = (seq_t)12;
        if ((n / 2) & 1U)          /* n/2 odd */
            return base - (seq_t)1;
        else                       /* n/2 even */
            return base + (seq_t)1;
    }
}
