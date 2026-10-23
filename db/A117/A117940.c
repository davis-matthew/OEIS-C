/*
 * A117940  a(0)=1, thereafter a(3n) = a(3n+1)/3 = a(n), a(3n+2)=0.
 * https://oeis.org/A117940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(0)=1; a(3n)=a(n); a(3n+1)=3*a(n); a(3n+2)=0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n != 0)
    {
        idx_t r = n % 3;
        if (r == 2)
            return 0;
        if (r == 1)
            res *= 3;
        n /= 3;
    }
    return res;
}
