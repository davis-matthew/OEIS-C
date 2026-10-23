/*
 * A010785  Repdigit numbers, or numbers whose digits are all equal.
 * https://oeis.org/A010785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 0 if n==0; otherwise the digit d = ((n-1)%9)+1 is
 *            repeated len = ((n-1)/9)+1 times to form the decimal repdigit
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 84, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t k    = n - 1;
    idx_t len  = k / 9 + 1;
    seq_t digit = (seq_t)(k % 9) + 1;

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < len; ++i)
    {
        result = result * 10 + digit;
    }
    return result;
}
