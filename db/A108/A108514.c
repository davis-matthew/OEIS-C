/*
 * A108514  If n is a power of 2, a(n)=n; otherwise a(n) = (p-1)*n/p where p =
 *          smallest odd prime divisor of n.
 * https://oeis.org/A108514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is a power of 2; otherwise a(n) = n - n/p where p
 *            is the smallest odd prime divisor of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* guard, not used for offset 1 */

    seq_t n_seq = n;

    /* check for power of two (including n == 1) */
    if ((n & (n - 1)) == 0)
        return n_seq;

    /* isolate the odd part of n */
    idx_t temp = n;
    while ((temp % 2) == 0)
        temp /= 2;                       /* temp is odd > 1 */

    /* find smallest odd prime divisor of the odd part */
    idx_t p = 0;
    for (idx_t i = 3; i <= temp / i; i += 2)
    {
        if (temp % i == 0)
        {
            p = i;
            break;
        }
    }
    if (p == 0)            /* no divisor found – odd part itself is prime */
        p = temp;

    seq_t p_seq = p;
    return n_seq - n_seq / p_seq;
}
