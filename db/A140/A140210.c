/*
 * A140210  a(n) = Product_{h == 1 (mod 4) and h|n} h.
 * https://oeis.org/A140210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Product of divisors h of n such that h ≡ 1 (mod 4)
 * Range:     u8 n <= 44, u16 n <= 224, u32 n <= 584, u64 n <= 2924,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    /* Safety: n should be >= 1 according to OEIS offset, but guard anyway */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (d % 4 == 1)
                res *= (seq_t)d;

            idx_t other = n / d;
            if (other != d && other % 4 == 1)
                res *= (seq_t)other;
        }
    }
    return res;
}
