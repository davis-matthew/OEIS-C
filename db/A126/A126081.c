/*
 * A126081  a(n) = number of k, 1 <= k <= n, such that k divides ceiling(n/k).
 * https://oeis.org/A126081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 <= k <= n, such that k divides ceil(n/k)
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t q = (seq_t)(n / k);
        seq_t r = (seq_t)(n % k);
        if (r == 0)
        {
            if ((q % (seq_t)k) == 0)
                ++count;
        }
        else
        {
            if (((q + (seq_t)1) % (seq_t)k) == 0)
                ++count;
        }
    }
    return count;
}
