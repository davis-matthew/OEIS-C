/*
 * A037466  a(n)=Sum{d(i)*7^i: i=0,1,...,m}, where Sum{d(i)*5^i: i=0,1,...,m}
 *          is the base 5 representation of n.
 * https://oeis.org/A037466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the base‑5 representation of n as a base‑7
 *            number
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow   = 1;
    idx_t t = n;
    while (t)
    {
        idx_t digit = t % 5;
        result += (seq_t)digit * pow;
        pow *= 7;
        t /= 5;
    }
    return result;
}
