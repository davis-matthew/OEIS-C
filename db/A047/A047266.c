/*
 * A047266  Numbers that are congruent to {0, 1, 5} mod 6.
 * https://oeis.org/A047266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 6k-1 a(3k-1) = 6k-5 a(3k-2) = 6k-6 (n≥1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ceil(n/3) */
    idx_t k = n / 3;
    if (n % 3 != 0)
        ++k;

    switch (n % 3)
    {
        case 0:  /* n = 3k */
            return (seq_t)6 * (seq_t)k - (seq_t)1;
        case 1:  /* n = 3k-2 */
            return (seq_t)6 * (seq_t)k - (seq_t)6;
        case 2:  /* n = 3k-1 */
            return (seq_t)6 * (seq_t)k - (seq_t)5;
        default: /* unreachable */
            return 0;
    }
}
