/*
 * A217219  Theta series of planar hexagonal net (honeycomb) with respect to
 *          deep hole.
 * https://oeis.org/A217219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * Σ_{d|n} f(d), where * f(d) = 1 if d ≡ 1 (mod 3) * -1
 *            if d ≡ 2 (mod 3) * 0 if 3 | d * a(0) = 0 and a(n) = 0 whenever 3
 *            divides n. * The function implements this formula directly by
 *            enumerating * the divisors of n
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    // If 3 divides n, the series is zero.
    if (n % 3 == 0) return (seq_t)0;

    /* compute the signed sum over divisors */
    int64_t diff = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (d1 % 3 == 1) diff += 1;
            else if (d1 % 3 == 2) diff -= 1;

            if (d2 != d1)
            {
                if (d2 % 3 == 1) diff += 1;
                else if (d2 % 3 == 2) diff -= 1;
            }
        }
    }
    // Multiply by 6 to obtain the desired term.
    return (seq_t)(6 * diff);
}
