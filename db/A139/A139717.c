/*
 * A139717  If k is the smallest divisor of n that is >= sqrt(n) then a(n) =
 *          k^2 - n.
 * https://oeis.org/A139717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^2 - n, where k is the smallest divisor of n that is *
 *            >= sqrt(n). Equivalently, let d be the largest divisor of n that
 *            is * <= sqrt(n); then k = n / d
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
 
    /* Compute floor sqrt(n) without using external utilities. */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; i <= n / i; ++i)
        sqrt_n = i;
 
    /* Find the largest divisor <= sqrt(n). */
    idx_t d = 0;
    for (idx_t i = sqrt_n; ; --i) /* i will never underflow, break at i==1 */
    {
        if (n % i == 0)
        {
            d = i;
            break;
        }
    }
 
    /* Compute k = n / d and then a(n) = k^2 - n. */
    seq_t k  = (seq_t)n / (seq_t)d;
    seq_t sn = (seq_t)n;
    seq_t res = k * k - sn;
    return res;
}
