/*
 * A226785  If n=0 (mod 7) then a(n)=0, otherwise a(n)=7^(-1) in Z/nZ*.
 * https://oeis.org/A226785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n%7==0 then a(n)=0 else a(n)=inverse of 7 modulo n (a(1)=0)
 * Range:     u8 n <= 301, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;              /* a(1) is defined as 0 */
    if (n % 7 == 0) return 0;          /* if n is divisible by 7, a(n)=0 */

    /* Extended Euclid to find x such that 7*x ≡ 1 (mod n) */
    __int128 a_val  = 7;
    __int128 b_val  = n;
    __int128 x0 = 1, y0 = 0;
    __int128 x1 = 0, y1 = 1;

    while (b_val != 0) {
        __int128 q = a_val / b_val;
        __int128 r = a_val - q * b_val;

        __int128 x2 = x0 - q * x1;
        __int128 y2 = y0 - q * y1;

        a_val = b_val;  b_val = r;
        x0 = x1;        x1 = x2;
        y0 = y1;        y1 = y2;
    }

    /* At this point a_val == gcd(7,n) which is 1 */
    __int128 inv = x0 % (__int128)n;
    if (inv < 0) inv += n;

    return (seq_t)inv;
}
