/*
 * A045512  If decimal expansion of n is ab...d, a(n) = a^a + b^b + ... + d^d
 *          (ignoring any 0's).
 * https://oeis.org/A045512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of d^d over non‑zero decimal digits d of n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;            /* a(0) = 0 by definition */

    idx_t x = n;
    while (x > 0) {
        unsigned d = x % 10;          /* current decimal digit */
        x /= 10;
        if (d == 0)
            continue;                 /* ignore zeros */

        /* compute d^d (small exponent, d <= 9) */
        seq_t power = 1;
        for (unsigned i = 0; i < d; ++i)
            power *= (seq_t)d;

        sum += power;
    }
    return sum;
}
