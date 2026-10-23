/*
 * A047472  Numbers that are congruent to {0, 1, 3} (mod 8).
 * https://oeis.org/A047472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n + offset)/3) - const, using the pattern a(3k-2) =
 *            8k-8, a(3k-1) = 8k-7, a(3k) = 8k-5 (offset = 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;            /* n >= 1, r in {0,1,2} */
    seq_t k;

    if (r == 1) {               /* n = 3k - 2 */
        k = ((seq_t)n + 2) / 3;
        return 8 * k - 8;
    }
    if (r == 2) {               /* n = 3k - 1 */
        k = ((seq_t)n + 1) / 3;
        return 8 * k - 7;
    }
    /* r == 0, n = 3k */
    k = (seq_t)n / 3;
    return 8 * k - 5;
}
