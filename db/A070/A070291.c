/*
 * A070291  a(n) = lcm(10,n)/gcd(10,n).
 * https://oeis.org/A070291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(10,n)/gcd(10,n) = 10*n/(gcd(10,n)^2)
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1, guard against 0 */
        return 0;

    /* Compute gcd(10,n) using the last digit of n */
    idx_t mod10 = n % 10;
    idx_t g;
    if (mod10 == 0)
        g = 10;
    else if (mod10 == 5)
        g = 5;
    else if ((n & 1) == 0)     /* even and not a multiple of 5 */
        g = 2;
    else
        g = 1;

    /* a(n) = 10*n / (g*g)  = (n/g) * (10/g) */
    seq_t res = (seq_t)n;      /* n as seq_t */
    res /= g;                  /* n / g */
    seq_t factor = (seq_t)(10 / g);
    res *= factor;             /* multiply by 10/g */

    return res;
}
