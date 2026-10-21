/*
 * A059417  Start with 1; square; add 2; subtract 1; repeat.
 * https://oeis.org/A059417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0 * a(n) = a(n-1)^2 if n mod 3 == 1 * a(n) =
 *            a(n-1)+2 if n mod 3 == 2 * a(n) = a(n-1)-1 if n mod 3 == 0 *
 *            This corresponds to the cycle: square → add 2 → subtract 1 →
 *            repeat
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 15, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t v = (seq_t)1;
    const seq_t two = (seq_t)2;
    const seq_t one = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        switch (i % 3)
        {
            case 1:
                v = v * v;
                break;
            case 2:
                v = v + two;
                break;
            case 0:
                v = v - one;
                break;
        }
    }
    return v;
}
