/*
 * A123752  a(n) = 7*a(n-2), a(0) = 1, a(1) = 2.
 * https://oeis.org/A123752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 7*a(n-2), a(0) = 1, a(1) = 2
 */
seq_t a(idx_t n)
{
    /* Handle the two known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* k is floor(n/2); for even n, a(n)=7^k; for odd n, a(n)=2*7^k */
    idx_t k = n >> 1;               /* same as n / 2 for unsigned */
    seq_t res = ((n & 1) ? (seq_t)2 : (seq_t)1);

    /* Multiply by 7 k times – loop is O(n/2) which is trivial for n≤10000 */
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)7;

    return res;
}
