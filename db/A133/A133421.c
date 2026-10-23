/*
 * A133421  The "7x+1" map: x/p if p is the least prime factor < 7 of x, or
 *          7x+1 if there's no such p.
 * https://oeis.org/A133421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n even; else a(n) = n/3 if n%3==0; else a(n) = n/5
 *            if n%5==0; else a(n) = 7*n+1
 * Range:     u8 n <= 36, u16 n <= 9366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if (x % 2 == 0)
        return x / 2;
    if (x % 3 == 0)
        return x / 3;
    if (x % 5 == 0)
        return x / 5;
    return 7 * x + 1;
}
