/*
 * A033103  Number of 5's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=2}^{n+1} (number of digits equal to 5 in the base‑b
 *            representation of n)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Iterate over all admissible bases */
    for (idx_t b = 2; b <= n + 1; ++b) {
        idx_t m = n;

        /* Extract digits in base b and tally the 5’s */
        while (m) {
            if (m % b == 5)
                ++count;
            m /= b;
        }
    }

    return count;
}
