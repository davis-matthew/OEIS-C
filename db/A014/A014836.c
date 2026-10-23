/*
 * A014836  Sum modulo n of all the digits of n in every base from 2 to n-1.
 * https://oeis.org/A014836
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum_{b=2}^{n-1} digit_sum_in_base(n,b)) mod n
 * Range:     u8 n <= 285, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where digit_sum_in_base(n,b) is the sum of the digits of n in base b. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined for n < 3, but reasonable default */

    seq_t mod_sum = 0;
    for (idx_t base = 2; base < n; ++base)
    {
        seq_t tmp = (seq_t)n;
        seq_t dsum = 0;
        while (tmp)
        {
            dsum += tmp % base;
            tmp /= base;
        }
        mod_sum += dsum;
        mod_sum %= n; /* keep within bounds of seq_t */
    }

    return mod_sum;
}
