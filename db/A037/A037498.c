/*
 * A037498  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2.
 * https://oeis.org/A037498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; a(n)=5*a(n-1)+{1,0,2}[(n-1)%3], n≥2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑5 digits are the first n terms of the periodic sequence 1,0,2 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset 1 – not used but safe for n=0 */

    static const seq_t pattern[3] = {1, 0, 2};

    seq_t val = 1;                /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * 5 + pattern[(unsigned)((i - 1) % 3)];
    }

    return val;
}
